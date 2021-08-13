#ifndef GUI_CPP
#define GUI_CPP

struct GUI
{
    s32 id;
    u32 color;
    Rect pos;
};

vector<GUI *> GUIs;

s32 w = 40;
s32 h = 40;
s32 off = 10;

Rect bounds = {0, off, w, h};

void fixGUIs()
{
    s32 x = globalVariables.screenWidth - (w + off);
    bounds = {x, off, x + w, off + h};

    for (GUI *gui : GUIs)
    {
        gui->pos = {bounds.left, bounds.bottom - h, bounds.right, bounds.bottom};
        bounds.bottom += h + off;
    }
}

void generateGUI(Element *e)
{
    GUI *gui = new GUI();

    gui->id = e->id;
    gui->color = e->color;

    GUIs.push_back(gui);

    delete e;
}

s32 testForGUIs()
{
    Vector m = globalVariables.mouse;
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