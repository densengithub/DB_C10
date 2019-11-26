#include <assert.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>
#include "ite/itu.h"
#include "itu_cfg.h"
#include "itu_private.h"

static const char bgbtnName[] = "ITUBackgroundButton";

bool ituBackgroundButtonUpdate(ITUWidget* widget, ITUEvent ev, int arg1, int arg2, int arg3)
{
    bool result = false;
    ITUBackgroundButton* bgbtn = (ITUBackgroundButton*) widget;
    assert(bgbtn);

    result |= ituIconUpdate(widget, ev, arg1, arg2, arg3);

    if (ev == ITU_EVENT_MOUSEDOWN)
    {
		int x = arg2 - widget->rect.x;
		int y = arg3 - widget->rect.y;

		if (ituWidgetIsEnabled(widget) && ituWidgetIsInside(widget, x, y))
		{
			bgbtn->mouseLogging = 1;
			bgbtn->currentX = x;
			bgbtn->currentY = y;
		}

        if (ituWidgetIsEnabled(widget) && !result)
        {
            if (ituWidgetIsInside(widget, x, y))
            {
                result |= ituExecActions((ITUWidget*)bgbtn, bgbtn->actions, ev, 0);
            }
            else
            {
                result |= ituExecActions((ITUWidget*)bgbtn, bgbtn->actions, ITU_EVENT_MOUSEDOWN_OUTSIDE, 0);
            }
        }
    }
    else if (ev == ITU_EVENT_MOUSEUP)
    {
		int x = arg2 - widget->rect.x;
		int y = arg3 - widget->rect.y;

		bgbtn->mouseLogging = 0;

		if (ituWidgetIsEnabled(widget) && ituWidgetIsInside(widget, x, y))
		{
			bgbtn->currentX = x;
			bgbtn->currentY = y;
		}

        if (ituWidgetIsEnabled(widget) && ituWidgetIsVisible(widget))
        {
            if (ituWidgetIsInside(widget, x, y))
            {
                result |= ituExecActions((ITUWidget*)bgbtn, bgbtn->actions, ev, 0);
            }
            else
            {
                result |= ituExecActions((ITUWidget*)bgbtn, bgbtn->actions, ITU_EVENT_MOUSEUP_OUTSIDE, 0);
            }
        }
    }
	else if (ev == ITU_EVENT_MOUSEMOVE)
	{
		int x = arg2 - widget->rect.x;
		int y = arg3 - widget->rect.y;

		if (bgbtn->mouseLogging)
		{
			if (ituWidgetIsEnabled(widget) && ituWidgetIsInside(widget, x, y))
			{
				bgbtn->currentX = x;
				bgbtn->currentY = y;
			}
		}
	}
    else if (ev == ITU_EVENT_MOUSEDOUBLECLICK || ev == ITU_EVENT_MOUSELONGPRESS ||
        ev == ITU_EVENT_TOUCHSLIDELEFT || ev == ITU_EVENT_TOUCHSLIDEUP || ev == ITU_EVENT_TOUCHSLIDERIGHT || ev == ITU_EVENT_TOUCHSLIDEDOWN)
    {
        if (ituWidgetIsEnabled(widget) && !result)
        {
            int x = arg2 - widget->rect.x;
            int y = arg3 - widget->rect.y;

            if (!widget->rect.width || !widget->rect.height || ituWidgetIsInside(widget, x, y))
            {
                result |= ituExecActions(widget, bgbtn->actions, ev, arg1);
            }
        }
    }
    else if (ev >= ITU_EVENT_CUSTOM || ev == ITU_EVENT_TIMER)
    {
        if (ituWidgetIsEnabled(widget))
        {
            result |= ituExecActions((ITUWidget*)bgbtn, bgbtn->actions, ev, arg1);
        }
    }
    else if (ituWidgetIsActive(widget) && ituWidgetIsEnabled(widget) && !result)
    {
        switch (ev)
        {
        case ITU_EVENT_KEYDOWN:
            if (arg1 == ituScene->enterKey)
            {
                ituFocusWidget(bgbtn);
                result |= ituExecActions((ITUWidget*)bgbtn, bgbtn->actions, ev, arg1);
            }
            break;

        case ITU_EVENT_KEYUP:
            if (arg1 == ituScene->enterKey)
            {
                result |= ituExecActions((ITUWidget*)bgbtn, bgbtn->actions, ev, arg1);
            }
            break;
        }
    }

    return result;
}

void ituBackgroundButtonOnAction(ITUWidget* widget, ITUActionType action, char* param)
{
    ITUBackgroundButton* bgbtn = (ITUBackgroundButton*) widget;
    assert(bgbtn);

    switch (action)
    {
    case ITU_ACTION_DODELAY0:
        ituExecActions(widget, bgbtn->actions, ITU_EVENT_DELAY0, atoi(param));
        break;

    case ITU_ACTION_DODELAY1:
        ituExecActions(widget, bgbtn->actions, ITU_EVENT_DELAY1, atoi(param));
        break;

    case ITU_ACTION_DODELAY2:
        ituExecActions(widget, bgbtn->actions, ITU_EVENT_DELAY2, atoi(param));
        break;

    case ITU_ACTION_DODELAY3:
        ituExecActions(widget, bgbtn->actions, ITU_EVENT_DELAY3, atoi(param));
        break;

    case ITU_ACTION_DODELAY4:
        ituExecActions(widget, bgbtn->actions, ITU_EVENT_DELAY4, atoi(param));
        break;

    case ITU_ACTION_DODELAY5:
        ituExecActions(widget, bgbtn->actions, ITU_EVENT_DELAY5, atoi(param));
        break;

    case ITU_ACTION_DODELAY6:
        ituExecActions(widget, bgbtn->actions, ITU_EVENT_DELAY6, atoi(param));
        break;

    case ITU_ACTION_DODELAY7:
        ituExecActions(widget, bgbtn->actions, ITU_EVENT_DELAY7, atoi(param));
        break;

    default:
        ituWidgetOnActionImpl(widget, action, param);
        break;
    }
}

void ituBackgroundButtonInit(ITUBackgroundButton* bgbtn)
{
    assert(bgbtn);
    ITU_ASSERT_THREAD();

    memset(bgbtn, 0, sizeof (ITUBackgroundButton));

    ituBackgroundInit(&bgbtn->bg);

    ituWidgetSetType(bgbtn, ITU_BACKGROUNDBUTTON);
    ituWidgetSetName(bgbtn, bgbtnName);
    ituWidgetSetUpdate(bgbtn, ituBackgroundButtonUpdate);
    ituWidgetSetOnAction(bgbtn, ituBackgroundButtonOnAction);
}

void ituBackgroundButtonLoad(ITUBackgroundButton* bgbtn, uint32_t base)
{
	assert(bgbtn);

    ituBackgroundLoad(&bgbtn->bg, base);

    ituWidgetSetUpdate(bgbtn, ituBackgroundButtonUpdate);
    ituWidgetSetOnAction(bgbtn, ituBackgroundButtonOnAction);
}


void ituBackgroundButtonMouseDownXY(ITUWidget* widget, int* x, int* y)
{
	ITUBackgroundButton* bb = (ITUBackgroundButton*)widget;
	
	ITU_ASSERT_THREAD();

	//*x = (xy >> 16);
	//*y = (xy << 16) >> 16;
	if (bb)
	{
		*x = bb->currentX;
		*y = bb->currentY;
	}
	else
	{
		*x = -1;
		*y = -1;
	}
}