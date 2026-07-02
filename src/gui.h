#ifndef GUI_H
#define GUI_H

enum GuiAction {
    GUI_ACTION_NONE = 0,
    GUI_ACTION_RUN_PAUSE = 1,
    GUI_ACTION_STEP = 2,
    GUI_ACTION_RESET = 3
};

class Gui {
public:
    Gui();

    // Open simulator window
    void open();

    // Check if user closed window
    bool shouldClose();

    // Draw full GUI and return button action
    GuiAction draw(
        const char testStatus[],
        const char consoleText[],
        int frameNumber,
        bool running,
        bool halted,
        unsigned short pc
    );

    // Close simulator window
    void close();

private:
    int width;
    int height;

    void drawStatusPanel(
        const char testStatus[],
        int frameNumber,
        bool running,
        bool halted,
        unsigned short pc
    );

    void drawConsolePanel(const char consoleText[]);

    GuiAction drawControlPanel(bool running, bool halted);

    bool drawButton(float x, float y, float w, float h, const char text[]);
};

#endif