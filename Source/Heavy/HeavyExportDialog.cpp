/*
 // Copyright (c) 2022 Timothy Schoen and Wasted Audio
 // For information on usage and redistribution, and for a DISCLAIMER OF ALL
 // WARRANTIES, see the file, "LICENSE.txt," in this distribution.
 */
#include <juce_gui_basics/juce_gui_basics.h>
#include "Utility/Config.h"
#include "Utility/Fonts.h"

#include "Dialogs/Dialogs.h"
#include "HeavyExportDialog.h"
// #include "Dialogs/HelpDialog.h"

#include "PluginEditor.h"
#include "Components/PropertiesPanel.h"
#include "Utility/OSUtils.h"

#include "Toolchain.h"
#include "ExportingProgressView.h"
#include "ExporterBase.h"
#include "CppExporter.h"
#include "DPFExporter.h"
#include "DaisyExporter.h"
#include "OWLExporter.h"
#include "PdExporter.h"
#include "WASMExporter.h"

// Placeholder ESP32 exporter view
class ESP32Exporter final : public ExporterBase {
public:
    ESP32Exporter(PluginEditor* editor, ExportingProgressView* exportingView)
        : ExporterBase(editor, exportingView)
    {
        placeholder.setText("ESP32 export is coming soon", dontSendNotification);
        placeholder.setJustificationType(Justification::centred);
        placeholder.setColour(Label::textColourId, findColour(PlugDataColour::panelTextColourId));
        addAndMakeVisible(placeholder);

        // PoC: sempre permitir clicar em Export nesta opção
        validPatchSelected = true;
        unsavedLabel.setVisible(false);
        exportButton.setEnabled(true);
    }

    // Minimal state plumbing
    ValueTree getState() override { return ValueTree("ESP32Exporter"); }
    void setState(ValueTree& /*state*/) override { }

private:
    Label placeholder;

    void resized() override {
        // Call base layout to place panel and export button
        ExporterBase::resized();
        // Keep placeholder at the top without covering controls
        placeholder.setBounds(0, 0, getWidth(), 24);
    }

    bool performExport(String const& pdPatch, String const& outdir, String const& name,
               String const& copyright, StringArray const& searchPaths) override
    {
        // 1) Determina um diretório de saída gravável
        File requestedOutDir(outdir);
        File effectiveOutDir = requestedOutDir;
        if (!effectiveOutDir.exists() || !effectiveOutDir.isDirectory()) {
            effectiveOutDir.createDirectory();
        }
        if (effectiveOutDir.isRoot() || !effectiveOutDir.hasWriteAccess()) {
            // Fallback: ~/plugdata_esp32_exports/<name>
            File fallbackBase = File::getSpecialLocation(File::userHomeDirectory).getChildFile("plugdata_esp32_exports");
            fallbackBase.createDirectory();
            effectiveOutDir = fallbackBase.getChildFile(name.isNotEmpty() ? name : "Untitled");
            effectiveOutDir.createDirectory();
        }

        // 2) Executa hvcc com o patch selecionado, gerando saída em effectiveOutDir
        if (exportingView)
            exportingView->logToConsole("ESP32: executando hvcc...\nSaída: " + effectiveOutDir.getFullPathName() + "\n");

#if JUCE_WINDOWS
        auto const heavyPath = heavyExecutable.getFullPathName().replaceCharacter('\\', '/');
#else
        auto const heavyPath = heavyExecutable.getFullPathName();
#endif

    StringArray args = { heavyPath.quoted(), pdPatch.quoted(), "-o", effectiveOutDir.getFullPathName().quoted() };
        args.add("-n" + name);

        if (copyright.isNotEmpty()) {
            args.add("--copyright");
            args.add(copyright.quoted());
        }

        args.add("-v");
        args.add("-p");
        for (auto& path : searchPaths) {
            args.add(path);
        }

        if (shouldQuit)
            return true;

        auto const command = args.joinIntoString(" ");
        exportingView->logToConsole("Command: " + command + "\n");
        Toolchain::startShellScript(command, this);

        waitForProcessToFinish(-1);
        exportingView->flushConsole();

        if (shouldQuit)
            return true;

        // 3) Limpa a pasta de destino e depois copia o conteúdo
        File outputDir = effectiveOutDir;
        File cDir = outputDir.getChildFile("c");
        File targetDir = File("/home/vinicius/Projects/ime-embarcados-lib/c");
        if (exportingView)
            exportingView->logToConsole("ESP32: limpando " + targetDir.getFullPathName() + "...\n");
        if (targetDir.exists())
            targetDir.deleteRecursively();
        targetDir.createDirectory();

        if (exportingView)
            exportingView->logToConsole("ESP32: copiando conteúdo de 'c' para " + targetDir.getFullPathName() + "...\n");

        if (cDir.isDirectory()) {
            DirectoryIterator it(cDir, false, "*", File::findFilesAndDirectories);
            while (it.next()) {
                auto src = it.getFile();
                auto dst = targetDir.getChildFile(src.getFileName());
                if (src.isDirectory()) {
                    src.copyDirectoryTo(dst);
                } else {
                    src.copyFileTo(dst);
                }
            }
        } else {
            if (exportingView)
                exportingView->logToConsole("Aviso: diretório 'c' não encontrado em " + outputDir.getFullPathName() + "\n");
        }

        // Limpa pastas intermediárias geradas pelo hvcc (opcional)
        outputDir.getChildFile("ir").deleteRecursively();
        outputDir.getChildFile("hv").deleteRecursively();

        // Delay curto para garantir código de saída correto
        Time::waitForMillisecondCounter(Time::getMillisecondCounter() + 300);

        return getExitCode();
    }

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ESP32Exporter)
};

class ExporterSettingsPanel final : public Component
    , private ListBoxModel {
public:
    ListBox listBox;
    int listBoxWidth = 160;

    TextButton addButton = TextButton(Icons::Add);

    OwnedArray<ExporterBase> views;

    std::function<void(int)> onChange;

    StringArray items = {
        "C++ Code",
        "Electro-Smith Daisy",
        "DPF Audio Plugin",
        "OWL Platform",
        "Pd External",
        "WebAssembly",
        "ESP32"
    };

    ExporterSettingsPanel(PluginEditor* editor, ExportingProgressView* exportingView)
    {
        addChildComponent(views.add(new CppExporter(editor, exportingView)));
        addChildComponent(views.add(new DaisyExporter(editor, exportingView)));
        addChildComponent(views.add(new DPFExporter(editor, exportingView)));
        addChildComponent(views.add(new OWLExporter(editor, exportingView)));
        addChildComponent(views.add(new PdExporter(editor, exportingView)));
        addChildComponent(views.add(new WASMExporter(editor, exportingView)));
        addChildComponent(views.add(new ESP32Exporter(editor, exportingView)));

        addAndMakeVisible(listBox);

        listBox.setModel(this);
        listBox.setOutlineThickness(0);
        listBox.selectRow(0);
        listBox.setColour(ListBox::backgroundColourId, Colours::transparentBlack);
        listBox.setRowHeight(28);

        restoreState();
    }

    ~ExporterSettingsPanel() override
    {
        saveState();
    }

    ValueTree getState() const
    {
        ValueTree stateTree("HeavySelect");
        stateTree.setProperty("listBox", listBox.getSelectedRow(), nullptr);
        return stateTree;
    }

    void setState(ValueTree& stateTree)
    {
        auto const tree = stateTree.getChildWithName("HeavySelect");
        listBox.selectRow(tree.getProperty("listBox"));
    }

    void restoreState()
    {
        auto const settingsTree = SettingsFile::getInstance()->getValueTree();
        auto heavyState = settingsTree.getChildWithName("HeavyState");
        if (heavyState.isValid()) {
            this->setState(heavyState);
            for (int i = 0; i < views.size(); i++) {
                views[i]->blockDialog = true;
                views[i]->setState(heavyState);
                views[i]->blockDialog = false;
            }
        }
    }

    ValueTree saveState()
    {
        ValueTree state("HeavyState");
        state.appendChild(this->getState(), nullptr);
        state.appendChild(views[0]->getState(), nullptr);
        state.appendChild(views[1]->getState(), nullptr);
        state.appendChild(views[2]->getState(), nullptr);
        state.appendChild(views[3]->getState(), nullptr);
        state.appendChild(views[4]->getState(), nullptr);
        state.appendChild(views[5]->getState(), nullptr);
        state.appendChild(views[6]->getState(), nullptr);

        auto settingsTree = SettingsFile::getInstance()->getValueTree();

        auto const oldState = settingsTree.getChildWithName("HeavyState");
        if (oldState.isValid()) {
            settingsTree.removeChild(oldState, nullptr);
        }
        settingsTree.appendChild(state, nullptr);

        return state;
    }

    void paint(Graphics& g) override
    {
        auto const listboxBounds = getLocalBounds().removeFromLeft(listBoxWidth);

        Path p;
        p.addRoundedRectangle(listboxBounds.getX(), listboxBounds.getY(), listboxBounds.getWidth(), listboxBounds.getHeight(), Corners::windowCornerRadius, Corners::windowCornerRadius, false, false, true, false);

        g.setColour(findColour(PlugDataColour::sidebarBackgroundColourId));
        g.fillPath(p);
    }

    void paintOverChildren(Graphics& g) override
    {
        auto const listboxBounds = getLocalBounds().removeFromLeft(listBoxWidth);

        g.setColour(findColour(PlugDataColour::toolbarOutlineColourId));
        g.drawLine(Line<float>(listboxBounds.getTopRight().toFloat(), listboxBounds.getBottomRight().toFloat()));
    }

    void selectedRowsChanged(int const lastRowSelected) override
    {
        for (auto* view : views) {
            // Make sure we remember common values when switching views
            if (view->isVisible()) {
                views[lastRowSelected]->patchFile = view->patchFile;
                views[lastRowSelected]->projectNameValue = view->projectNameValue.getValue();
                views[lastRowSelected]->projectCopyrightValue = view->projectCopyrightValue.getValue();

                views[lastRowSelected]->blockDialog = true;
                views[lastRowSelected]->inputPatchValue = view->inputPatchValue.getValue();
                views[lastRowSelected]->blockDialog = false;
            }
            view->setVisible(false);
        }

        views[lastRowSelected]->setVisible(true);
    }

    void resized() override
    {
        auto b = getLocalBounds();
        listBox.setBounds(b.removeFromLeft(listBoxWidth).reduced(4));

        for (auto* view : views) {
            view->setBounds(b);
        }
    }

    int getNumRows() override
    {
        return items.size();
    }

    void paintListBoxItem(int const row, Graphics& g, int const width, int const height, bool const rowIsSelected) override
    {
        if (isPositiveAndBelow(row, items.size())) {
            if (rowIsSelected) {
                g.setColour(findColour(PlugDataColour::sidebarActiveBackgroundColourId));
                g.fillRoundedRectangle(Rectangle<float>(3, 3, width - 6, height - 6), Corners::defaultCornerRadius);
            }

            auto const textColour = findColour(PlugDataColour::sidebarTextColourId);

            Fonts::drawText(g, items[row], Rectangle<int>(15, 0, width - 30, height), textColour, 15);
        }
    }

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ExporterSettingsPanel)
};

HeavyExportDialog::HeavyExportDialog(Dialog* dialog)
    : exportingView(new ExportingProgressView())
    , installer(new ToolchainInstaller(dynamic_cast<PluginEditor*>(dialog->parentComponent), dialog))
    , exporterPanel(new ExporterSettingsPanel(dynamic_cast<PluginEditor*>(dialog->parentComponent), exportingView.get()))
    , infoButton(new MainToolbarButton(Icons::Help))
{
    hasToolchain = Toolchain::dir.exists();

    // Don't do this relative to toolchain variable, that won't work on Windows
    auto const versionFile = ProjectInfo::appDataDir.getChildFile("Toolchain").getChildFile("VERSION");
    auto const installedVersion = versionFile.loadFileAsString().trim().removeCharacters(".").getIntValue();

    // Create integer versions by removing the dots
    // Compare latest version on github to the currently installed version
    int latestVersion;
    try {
        auto const compatTable = JSON::parse(URL("https://raw.githubusercontent.com/plugdata-team/plugdata-heavy-toolchain/main/COMPATIBILITY").readEntireTextStream());
        // Get latest version
        if (compatTable.isObject()) {
            latestVersion = compatTable.getDynamicObject()->getProperty(String(ProjectInfo::versionString).upToFirstOccurrenceOf("-", false, false)).toString().removeCharacters(".").getIntValue();
        } else {
            latestVersion = installedVersion;
        }
    }
    // Network error, JSON error or empty version string somehow
    catch (...) {
        latestVersion = installedVersion;
        return;
    }

    if (hasToolchain && latestVersion > installedVersion) {
        installer->needsUpdate = true;
        hasToolchain = false;
    }

    addChildComponent(*installer);
    addChildComponent(*exporterPanel);
    addChildComponent(*exportingView);

    exportingView->setAlwaysOnTop(true);

    /*
    infoButton->onClick = [this]() {
        helpDialog = std::make_unique<HelpDialog>(nullptr);
        helpDialog->onClose = [this]() {
            helpDialog.reset(nullptr);
        };
    }; */
    infoButton->onClick = [] {
        URL("https://wasted-audio.github.io/hvcc/docs/01.introduction.html#what-is-heavy").launchInDefaultBrowser();
    };
    addAndMakeVisible(*infoButton);

    installer->toolchainInstalledCallback = [this] {
        hasToolchain = true;
        exporterPanel->setVisible(true);
        installer->setVisible(false);
    };

    if (hasToolchain) {
        exporterPanel->setVisible(true);
    } else {
        installer->setVisible(true);
    }
}

HeavyExportDialog::~HeavyExportDialog()
{
    Dialogs::dismissFileDialog();

    // Clean up temp files
    Toolchain::deleteTempFiles();
}

void HeavyExportDialog::paint(Graphics& g)
{
    g.setColour(findColour(PlugDataColour::panelBackgroundColourId));
    g.fillRoundedRectangle(getLocalBounds().toFloat(), Corners::windowCornerRadius);

    auto const titlebarBounds = getLocalBounds().removeFromTop(40);

    Path p;
    p.addRoundedRectangle(titlebarBounds.getX(), titlebarBounds.getY(), titlebarBounds.getWidth(), titlebarBounds.getHeight(), Corners::windowCornerRadius, Corners::windowCornerRadius, true, true, false, false);

    g.setColour(findColour(PlugDataColour::toolbarBackgroundColourId));
    g.fillPath(p);

    Fonts::drawStyledText(g, "Compiler", Rectangle<float>(0.0f, 4.0f, getWidth(), 32.0f), findColour(PlugDataColour::panelTextColourId), Semibold, 15, Justification::centred);
}

void HeavyExportDialog::paintOverChildren(Graphics& g)
{
    g.setColour(findColour(PlugDataColour::toolbarOutlineColourId));
    g.drawHorizontalLine(40, 0.0f, getWidth());
}

void HeavyExportDialog::resized()
{
    auto const b = getLocalBounds().withTrimmedTop(40);
    exporterPanel->setBounds(b);
    installer->setBounds(b);
    exportingView->setBounds(b);
    infoButton->setBounds(Rectangle<int>(40, 40));
}
