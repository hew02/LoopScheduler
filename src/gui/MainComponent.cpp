#include "sproj_2024/gui/MainComponent.h"
#include <cstdlib>
#include <focusrite/e2e/ComponentSearch.h>

/**
 * @brief Construct a new Main Component:: Main Component object
 * 
 * @param tree 
 * @param manager 
 * @param dmanager 
 */
MainComponent::MainComponent(const juce::ValueTree& tree, SPCommandManager& manager, juce::AudioDeviceManager& dmanager)
    : commandManager(manager), deviceManager(dmanager), valueTree(tree)

{
    setOpaque(true);


    // NOTE: old GUI stuff
    /*rulerDeckGUI = std::make_shared<RulerDeckGUI>();
    controlDeckGui = std::make_shared<ControlDeckGUI>(valueTree);
    freeDeckGui = std::make_shared<FreeDeckGUI>(valueTree);
    mainDeckHolder = std::make_shared<MainDeckHolder>(valueTree, *freeDeckGui);
    mixDeckGui = std::make_shared<MixDeckGUI>(valueTree);
    menu = std::make_shared<MenuComponent>(commandManager);
    deviceSelector = std::make_shared<DeviceSelectionMenu>(deviceManager, commandManager);

    addAndMakeVisible(rulerDeckGUI.get());
    addAndMakeVisible(controlDeckGui.get());
    addAndMakeVisible(freeDeckGui.get());
    addAndMakeVisible(mixDeckGui.get());
    addAndMakeVisible(mainDeckHolder.get());
    addAndMakeVisible(menu.get());
    addChildComponent(deviceSelector.get());*/

    // Set up opengl context
    glCtx.setOpenGLVersionRequired(juce::OpenGLContext::openGL3_2);
    glCtx.setRenderer(this);
    glCtx.attachTo(*this);
    glCtx.setContinuousRepainting(true);

    //focusrite::e2e::ComponentSearch::setTestId(*deviceSelector, "test_deviceSelector"); // for testing

    commandManager.registerAllCommandsForTarget(this);
    commandManager.addTargetToCommandManager(this);

    setSize(600, 400);
    setWantsKeyboardFocus(true);

    // TODO: ez create a new track
    //createNewTrack();
}

/**
 * @brief Destroy the Main Component:: Main Component object
 * 
 */
MainComponent::~MainComponent()
{

}

/**
 * @brief Creates a new track.
 * 
 */
void MainComponent::createNewTrack()
{
    juce::ValueTree newNode(SP_ID::TRACK);
    SP::createNewID(newNode);
    auto trackBranch = valueTree.getChildWithName(SP_ID::TRACK_BRANCH);
    trackBranch.appendChild(newNode, nullptr);
    mixDeckGui->addTrack(newNode);
    mainDeckHolder->addTrack(newNode);
}

/**
 * @brief 
 * 
 * @note not the fastest way of doing this (better to initialize this before hand)
 */
void MainComponent::createNewDummyClip()
{
    freeDeckGui->createNewDummyClip();
}

void MainComponent::startOrStopAnimation()
{
    mainDeckHolder->startOrStopAnimation();
}

void MainComponent::paint(juce::Graphics& g)
{
    // NOTE ImGui is now the backend
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    //g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
}

void MainComponent::resized()
{
    /*rulerDeckGUI->setBounds(getX(), getY(), getWidth(), controlDeckGui->getHeight());
    controlDeckGui->setBounds(getX(), getY(), mixDeckGui->getWidth(), controlDeckGui->getHeight());
    freeDeckGui->setBounds(getX(), getHeight() - freeDeckGui->getHeight(), getWidth(), freeDeckGui->getHeight());
    mixDeckGui->setBounds(getX(), getY() + controlDeckGui->getHeight(), mixDeckGui->getWidth(), getHeight()
                         - controlDeckGui->getHeight() - freeDeckGui->getHeight());
    mainDeckHolder->setBounds(getX() + mixDeckGui->getWidth(), getY() + controlDeckGui->getHeight(), getWidth()
                             - mixDeckGui->getWidth(),
                             getHeight() - freeDeckGui->getHeight() - controlDeckGui->getHeight());
    mainDeckHolder->resized();*/
}

void MainComponent::childBoundsChanged(Component* child)
{
    juce::ignoreUnused(child);

    /*controlDeckGui->setBounds(getX(), getY(), mixDeckGui->getWidth(), controlDeckGui->getHeight());

    freeDeckGui->setBounds(getX(), getHeight() - freeDeckGui->getHeight(), getWidth(), freeDeckGui->getHeight());

    mixDeckGui->setBounds(getX(), getY() + controlDeckGui->getHeight(), mixDeckGui->getWidth(),
                          getHeight() - controlDeckGui->getHeight() - freeDeckGui->getHeight());

    mainDeckHolder->setBounds(getX() + mixDeckGui->getWidth(), getY() + controlDeckGui->getHeight(),
                              getWidth() - mixDeckGui->getWidth(),
                              getHeight() - freeDeckGui->getHeight() - controlDeckGui->getHeight());*/
}

//ApplicationCommandTarget methods

juce::ApplicationCommandTarget* MainComponent::getNextCommandTarget()
{
    return nullptr;
}

void MainComponent::getAllCommands(juce::Array<juce::CommandID>& c)
{
    juce::Array<juce::CommandID> commands{
        SP_CommandID::createNewTrack,
        SP_CommandID::createNewDummyClip
    };
    c.addArray(commands);
}

void MainComponent::getCommandInfo(const juce::CommandID commandID, juce::ApplicationCommandInfo& result)
{
    switch (commandID)
    {
    case SP_CommandID::createNewTrack:
        result.setInfo("create new track", "creates gui/audio for new track", "Audio", 0);
        result.setTicked(false);
        result.addDefaultKeypress('t', juce::ModifierKeys::commandModifier);
        break;
    case SP_CommandID::createNewDummyClip:
        result.setInfo("create new Dummy Clip", "creates new dummy clip", "Audio", 0);
        result.setTicked(false);
        result.addDefaultKeypress('d', juce::ModifierKeys::commandModifier);
        break;
    default:
        break;
    }
}

bool MainComponent::perform(const InvocationInfo& info)
{
    switch (info.commandID)
    {
    case SP_CommandID::createNewTrack:
        createNewTrack();
        break;
    case SP_CommandID::createNewDummyClip:
        //createNewDummyClip();
        SP::printVT(valueTree);
        break;
    default:
        return false;
    }

    return true;
}



void MainComponent::newOpenGLContextCreated() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplJuce_Init(*this, glCtx);
    ImGui_ImplOpenGL3_Init();

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigWindowsMoveFromTitleBarOnly = true;
}

void MainComponent::renderOpenGL() {
    using namespace juce::gl;

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplJuce_NewFrame();
    ImGui::NewFrame();

    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem(MAIN_MENU_EXIT_ITEM)) {
                // TODO call a shutdown function
                exit(EXIT_SUCCESS);
            }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }

    ImGui::Begin("Deck", nullptr);
    {
        ImGui::LabelText("Hello, world", "d");
        ImGui::Bullet();
    }
    ImGui::End();

    ImGui::Begin("Track", nullptr);
    {
        //float *v = new float;
        //ImGui::DragFloat("", v);
    }
    ImGui::End();

	Rectangle<int> r = Desktop::getInstance().getDisplays().getMainDisplay().userArea;
    int w = r.getWidth();
	int h = r.getHeight();

    ImGui::SetNextWindowPos(ImVec2(0, h - 300));
    ImGui::SetNextWindowSize(ImVec2(w, 300));
    ImGui::Begin("Dump", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
    {
    }
    ImGui::End();


    //ImGui::ShowDemoWindow();

    ImGui::Render();

    // background begin
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    // background end

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void MainComponent::openGLContextClosing() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplJuce_Shutdown();
    ImGui::DestroyContext();
}
