/*
 // Copyright (c) 2022 Timothy Schoen and Wasted Audio
 // For information on usage and redistribution, and for a DISCLAIMER OF ALL
 // WARRANTIES, see the file, "LICENSE.txt," in this distribution.
 */
#pragma once

#include "ExporterBase.h"
#include "ExportingProgressView.h"
#include "Toolchain.h"
#include "Utility/OSUtils.h"
#include <juce_gui_basics/juce_gui_basics.h>

class ESP32Exportar final : public ExporterBase {
public:
    TextButton flashButton = TextButton("Flash");

    ESP32Exportar(PluginEditor* editor, ExportingProgressView* exportingView)
        : ExporterBase(editor, exportingView)
    {
        // Simplified panel: only a Flash button
        exportButton.setVisible(false);
        addAndMakeVisible(flashButton);

        auto const backgroundColour = findColour(PlugDataColour::panelBackgroundColourId);
        flashButton.setColour(TextButton::buttonColourId, backgroundColour.contrasting(0.05f));
        flashButton.setColour(TextButton::buttonOnColourId, backgroundColour.contrasting(0.1f));
        flashButton.setColour(ComboBox::outlineColourId, Colours::transparentBlack);

        flashButton.onClick = [this] {
            // Use a temp hvcc outdir; we will copy into project and build/flash from there
            auto const tempFolder = File::getSpecialLocation(File::tempDirectory).getChildFile("HeavyESP32-" + Uuid().toString().substring(10));
            Toolchain::deleteTempFileLater(tempFolder);
            startExport(tempFolder);
        };
    }

    void resized() override {
        ExporterBase::resized();
        flashButton.setBounds(exportButton.getBounds());
    }

    ValueTree getState() override {
        ValueTree stateTree("ESP32Exportar");
        stateTree.setProperty("inputPatchValue", getValue<String>(inputPatchValue), nullptr);
        stateTree.setProperty("projectNameValue", getValue<String>(projectNameValue), nullptr);
        stateTree.setProperty("projectCopyrightValue", getValue<String>(projectCopyrightValue), nullptr);
        return stateTree;
    }

    void setState(ValueTree& stateTree) override {
        auto const tree = stateTree.getChildWithName("ESP32Exportar");
        inputPatchValue = tree.getProperty("inputPatchValue");
        projectNameValue = tree.getProperty("projectNameValue");
        projectCopyrightValue = tree.getProperty("projectCopyrightValue");
    }

    bool performExport(String const& pdPatch, String const& outdir, String const& name,
                       String const& copyright, StringArray const& searchPaths) override
    {
        // 1) Determine hvcc output dir (writable)
        File requestedOutDir(outdir);
        File effectiveOutDir = requestedOutDir;
        if (!effectiveOutDir.exists() || !effectiveOutDir.isDirectory()) {
            effectiveOutDir.createDirectory();
        }
        if (effectiveOutDir.isRoot() || !effectiveOutDir.hasWriteAccess()) {
            File fallbackBase = File::getSpecialLocation(File::userHomeDirectory).getChildFile("plugdata_esp32_exports");
            fallbackBase.createDirectory();
            effectiveOutDir = fallbackBase.getChildFile(name.isNotEmpty() ? name : "Untitled");
            effectiveOutDir.createDirectory();
        }

        // 2) Run hvcc to generate Heavy C (force name to Untitled for ESP-IDF alignment)
        if (exportingView)
            exportingView->logToConsole("ESP32: executando hvcc...\nSaída: " + effectiveOutDir.getFullPathName() + "\n");

#if JUCE_WINDOWS
        auto const heavyPath = heavyExecutable.getFullPathName().replaceCharacter('\\', '/');
#else
        auto const heavyPath = heavyExecutable.getFullPathName();
#endif
        StringArray args = { heavyPath.quoted(), pdPatch.quoted(), "-o", effectiveOutDir.getFullPathName().quoted() };
        args.add("-nUntitled");

        if (copyright.isNotEmpty()) {
            args.add("--copyright");
            args.add(copyright.quoted());
        }

        args.add("-v");
        args.add("-p");
        for (auto& path : searchPaths) {
            args.add(path);
        }

        if (shouldQuit) return true;
        auto const command = args.joinIntoString(" ");
        exportingView->logToConsole("Command: " + command + "\n");
        Toolchain::startShellScript(command, this);
        waitForProcessToFinish(-1);
        exportingView->flushConsole();
        if (shouldQuit) return true;

        // 3) Copy Heavy C into ESP-IDF project
        File outputDir = effectiveOutDir;
        File cDir = outputDir.getChildFile("c");
        File targetDir = File("/home/vinicius/Projects/ime-embarcados-lib/c");
        if (exportingView)
            exportingView->logToConsole("ESP32: limpando " + targetDir.getFullPathName() + "...\n");
        if (targetDir.exists()) targetDir.deleteRecursively();
        targetDir.createDirectory();

        if (exportingView)
            exportingView->logToConsole("ESP32: copiando conteúdo de 'c' para " + targetDir.getFullPathName() + "...\n");

        if (cDir.isDirectory()) {
            DirectoryIterator it(cDir, false, "*", File::findFilesAndDirectories);
            while (it.next()) {
                auto src = it.getFile();
                auto dst = targetDir.getChildFile(src.getFileName());
                if (src.isDirectory()) src.copyDirectoryTo(dst);
                else src.copyFileTo(dst);
            }
        } else {
            if (exportingView)
                exportingView->logToConsole("Aviso: diretório 'c' não encontrado em " + outputDir.getFullPathName() + "\n");
        }

        // 3.5) Patch HvMessage.c formatting
        if (exportingView) exportingView->logToConsole("Iniciando (2/4) Patch formatting if needed\n");
        {
            auto hvMessageFile = targetDir.getChildFile("HvMessage.c");
            if (hvMessageFile.existsAsFile()) {
                auto content = hvMessageFile.loadFileAsString();
                bool changed = false;
                if (! content.contains("<inttypes.h>")) {
                    auto anchor = String("#include \"HvMessage.h\"");
                    int pos = content.indexOf(anchor);
                    if (pos >= 0) {
                        pos += anchor.length();
                        content = content.substring(0, pos) + "\n#include <inttypes.h>" + content.substring(pos);
                        changed = true;
                    }
                }
                if (content.contains("\"0x%X\"")) {
                    content = content.replace("\"0x%X\"", "\"0x%\" PRIX32");
                    changed = true;
                }
                if (changed) {
                    hvMessageFile.replaceWithText(content);
                    if (exportingView) exportingView->logToConsole("HvMessage.c patch aplicado com sucesso.\n");
                } else {
                    if (exportingView) exportingView->logToConsole("HvMessage.c já está compatível, nenhum patch necessário.\n");
                }
            } else {
                if (exportingView) exportingView->logToConsole("Aviso: HvMessage.c não encontrado em " + targetDir.getFullPathName() + "\n");
            }
        }

        // 3.6) Update main/CMakeLists.txt SRCS with ../c/*.c|*.cpp
        if (exportingView) exportingView->logToConsole("Iniciando (4/4) Sanity-check Heavy sources\n");
        {
            auto cmakeMain = File("/home/vinicius/Projects/ime-embarcados-lib/main/CMakeLists.txt");
            if (cmakeMain.existsAsFile()) {
                auto cmakeText = cmakeMain.loadFileAsString();
                int srci = cmakeText.indexOf("SRCS");
                int incli = cmakeText.indexOf(srci + 1, "INCLUDE_DIRS");
                if (srci >= 0 && incli > srci) {
                    String srcSection = cmakeText.substring(srci, incli);
                    StringArray candidates;
                    if (cDir.isDirectory()) {
                        DirectoryIterator dit(cDir, false, "*", File::findFiles);
                        while (dit.next()) {
                            auto f = dit.getFile();
                            auto ext = f.getFileExtension();
                            if (ext.equalsIgnoreCase(".c") || ext.equalsIgnoreCase(".cpp")) {
                                candidates.add("../c/" + f.getFileName());
                            }
                        }
                    }
                    StringArray toAdd;
                    for (auto& rel : candidates) {
                        if (! srcSection.contains(rel)) toAdd.add(rel);
                    }
                    if (toAdd.isEmpty()) {
                        if (exportingView) exportingView->logToConsole("Nenhum novo arquivo para adicionar em SRCS.\n");
                    } else {
                        String addBuf;
                        for (auto& rel : toAdd) {
                            addBuf += "         \"";
                            addBuf += rel;
                            addBuf += "\"\n";
                        }
                        cmakeText = cmakeText.substring(0, incli) + addBuf + cmakeText.substring(incli);
                        cmakeMain.replaceWithText(cmakeText);
                        if (exportingView) exportingView->logToConsole("Adicionados " + String(toAdd.size()) + " fontes hvcc ao SRCS.\n");
                    }
                } else {
                    if (exportingView) exportingView->logToConsole("Aviso: não foi possível localizar seção SRCS/INCLUDE_DIRS em CMakeLists.txt.\n");
                }
            } else {
                if (exportingView) exportingView->logToConsole("Aviso: main/CMakeLists.txt não encontrado para sanity-check.\n");
            }
        }

        // Clean hvcc intermediates
        outputDir.getChildFile("ir").deleteRecursively();
        outputDir.getChildFile("hv").deleteRecursively();
        int hvccExit = getExitCode();

        // 4) Source ESP-IDF, build and flash
        if (exportingView) exportingView->logToConsole("ESP32: sourcing ESP-IDF, build e flash...\n");
        exportingView->showState(ExportingProgressView::Flashing);

#if JUCE_WINDOWS
        int flashExit = 0;
#else
        StringArray argv;
        argv.add("/usr/bin/zsh");
        argv.add("-lc");
        argv.add(". \"$HOME/esp/esp-idf/export.sh\" >/dev/null 2>&1; cd /home/vinicius/Projects/ime-embarcados-lib; idf.py build | cat; idf.py flash; idf.py fullclean");
        bool started = start(argv);
        waitForProcessToFinish(-1);
        exportingView->flushConsole();
        int flashExit = started ? getExitCode() : 1;
#endif

        // Optional: log bin path if present
        {
            File binPath("/home/vinicius/Projects/ime-embarcados-lib/build/ime-embarcados-lib.bin");
            if (binPath.existsAsFile()) {
                exportingView->logToConsole("ESP32: bin gerado: " + binPath.getFullPathName() + " (" + String(binPath.getSize()) + " bytes)\n");
            }
        }

        Time::waitForMillisecondCounter(Time::getMillisecondCounter() + 300);
        return (hvccExit != 0) || (flashExit != 0);
    }

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ESP32Exportar)
};
