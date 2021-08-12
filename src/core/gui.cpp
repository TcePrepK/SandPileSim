#ifndef GUI_CPP
#define GUI_CPP

struct GUI
{
    s32 id;
    u32 color;
    RECT pos;
};

vector<GUI *> GUIs;

s32 w = 40;
s32 h = 40;
s32 off = 10;

RECT bounds = {0, off, w, h};

void fixGUIs()
{
    bounds = {globalVariables.screenWidth - (w + off), off, w, h};

    for (GUI *gui : GUIs)
    {
        gui->pos = {bounds.left, bounds.top, bounds.left + w, bounds.top + h};
        bounds.top += h + off;
    }
}

void generateGUI(s32 id, u32 color)
{
    bounds.left = globalVariables.screenWidth - (w + off);

    GUI *gui = new GUI();

    gui->id = id;
    gui->color = color;

    gui->pos = {bounds.left, bounds.top, bounds.left + w, bounds.top + h};
    bounds.top += h + off;

    GUIs.push_back(gui);
}

s32 testForGUIs()
{
    POINT m = globalVariables.mouse;
    if (m.x < bounds.left || m.x > bounds.right || m.y < bounds.top || m.y > bounds.bottom)
        return -1;

    for (s32 i = 0; i < GUIs.size(); i++)
    {
        GUI *gui = GUIs.at(i);
        if (gui->pos.left <= m.x && m.x <= gui->pos.right && gui->pos.top <= m.y && m.y <= gui->pos.bottom)
            return gui->id;
    }

    return globalVariables.currentMatID;
}

#endif // GUI_CPP