#include <assert.h>
#include <malloc.h>
#include <string.h>
#include <math.h>
#include "ite/itu.h"
#include "itu_cfg.h"
#include "itu_private.h"

#define STOPANYWHERE_TIMER_CYCLE_MAX  20         //17ms x 20 ~ 340ms
#define STOPANYWHERE_SLIDE_CHECK_TIMING  300     //300ms
#define STOPANYWHERE_SLIDE_REQUIRE_DIST  1800    //~X40 ~Y40
#define STOPANYWHERE_BOUNCE_RATIO 10 //10%
#define STOPANYWHERE_DRAGGING_EYEMOTION 0

#define STOPANYWHERE_DRAGGING_PIPE 0
#define STOPANYWHERE_DRAGGING_PIPE_SIZE 1000
#define STOPANYWHERE_MOUSEDOWN_LONGPRESS_DELAY 1000

#define TEST_ALIGN 0
#define STOPANYWHERE_ALIGN_SHIFT_FACTOR 3
#define STOPANYWHERE_ALIGN_AVG_CENTER 1

//Debug Switch List
#define STOPANYWHERE_DEBUG_SETXY 0
#define STOPANYWHERE_DEBUG_FIXXY 0

static const char sawName[] = "ITUStopAnywhere";

ITUWidget* StopAnywhereGetVisibleChild(ITUStopAnywhere* saw)
{
	ITCTree *child, *tree = (ITCTree*)saw;
	assert(tree);

	for (child = tree->child; child; child = child->sibling)
	{
		if (child)
		{
			ITUWidget* widget = (ITUWidget*)child;

			if (widget->visible)
			{
				return widget;
			}
		}
	}
	return NULL;
}

int ituStopAnywhereCheckChildSame(ITUWidget* widget, bool horizontal)
{
	ITCTree* tree = (ITCTree*)widget;
	ITCTree *child = tree->child;
	int baseunit = 0;

	for (child = tree->child; child; child = child->sibling)
	{
		ITUWidget* ww = (ITUWidget*)child;
		int checksize = 0;

		if (horizontal)
			checksize = ww->rect.width;
		else
			checksize = ww->rect.height;

		if (ww)
		{
			if ((baseunit != 0) && (checksize != baseunit))
				return 0;
			else
				baseunit = checksize;
		}
	}

	return baseunit;
}

int ituStopAnywhereFixX(ITUStopAnywhere* saw, int targetX)
{
	int result_value = targetX;

	if (saw)
	{
		ITUWidget* widget = (ITUWidget*)saw;
		ITUWidget* child = StopAnywhereGetVisibleChild(saw);

		if (widget && child)
		{
			bool check_result = false;

			if (saw->bounce == 1)
			{
				int baseunit = ituStopAnywhereCheckChildSame(child, true);
				int alignavgshiftsize = (widget->rect.width % baseunit) / 2;

				if (!STOPANYWHERE_ALIGN_AVG_CENTER && ((alignavgshiftsize % 2) != 0))
					alignavgshiftsize = 0;

				int left_side_boundary = (widget->rect.width / STOPANYWHERE_BOUNCE_RATIO) + alignavgshiftsize;
				int right_side_boundary = ((widget->rect.width * (100 - STOPANYWHERE_BOUNCE_RATIO)) / 100) - alignavgshiftsize;

				if (targetX > (0 + alignavgshiftsize))
				{
					saw->stopAnywhereFlags |= ITU_STOPANYWHERE_BOUNCING_L;
					saw->stopAnywhereFlags |= ITU_STOPANYWHERE_BOUNCING;

					if (targetX > left_side_boundary)
					{
						check_result = true;
						result_value = left_side_boundary;
					}
				}
				else if ((targetX + child->rect.width) < (widget->rect.width - alignavgshiftsize))
				{
					saw->stopAnywhereFlags |= ITU_STOPANYWHERE_BOUNCING_R;
					saw->stopAnywhereFlags |= ITU_STOPANYWHERE_BOUNCING;

					if ((targetX + child->rect.width) < right_side_boundary)
					{
						check_result = true;
						result_value = (right_side_boundary - child->rect.width);
					}
				}
				else
				{
					saw->stopAnywhereFlags &= ~ITU_STOPANYWHERE_BOUNCING;
					saw->stopAnywhereFlags &= ~ITU_STOPANYWHERE_BOUNCING_L;
					saw->stopAnywhereFlags &= ~ITU_STOPANYWHERE_BOUNCING_R;
				}
			}
			else
			{
				if (targetX > 0)
				{
					if (STOPANYWHERE_DEBUG_FIXXY)
						printf("[DEBUG] Func: %s [Fix X from %d to %d]\n", __func__, targetX, 0);

					check_result = true;
					result_value = 0;
				}
				else if ((targetX + child->rect.width) < widget->rect.width)
				{
					check_result = true;
					result_value = (widget->rect.width - child->rect.width);
				}
			}

			if (check_result)
			{
				//
			}

			return result_value;
		}
		else
		{
			printf("[ERROR] Func: %s [%d]\n", __func__, __LINE__);
			return result_value;
		}
	}
	else
	{
		printf("[ERROR] Func: %s [%d]\n", __func__, __LINE__);
		return result_value;
	}
}

int ituStopAnywhereFixY(ITUStopAnywhere* saw, int targetY)
{
	int result_value = targetY;

	if (saw)
	{
		ITUWidget* widget = (ITUWidget*)saw;
		ITUWidget* child = StopAnywhereGetVisibleChild(saw);

		if (widget && child)
		{
			bool check_result = false;

			if (saw->bounce == 2)
			{
				int top_side_boundary = (widget->rect.height / STOPANYWHERE_BOUNCE_RATIO);
				int bottom_side_boundary = ((widget->rect.height * (100 - STOPANYWHERE_BOUNCE_RATIO)) / 100);

				if (targetY > 0)
				{
					saw->stopAnywhereFlags |= ITU_STOPANYWHERE_BOUNCING_T;
					saw->stopAnywhereFlags |= ITU_STOPANYWHERE_BOUNCING;

					if (targetY > top_side_boundary)
					{
						check_result = true;
						result_value = top_side_boundary;
					}
				}
				else if ((targetY + child->rect.height) < widget->rect.height)
				{
					saw->stopAnywhereFlags |= ITU_STOPANYWHERE_BOUNCING_B;
					saw->stopAnywhereFlags |= ITU_STOPANYWHERE_BOUNCING;

					if ((targetY + child->rect.height) < bottom_side_boundary)
					{
						check_result = true;
						result_value = (bottom_side_boundary - child->rect.height);
					}
				}
				else
				{
					saw->stopAnywhereFlags &= ~ITU_STOPANYWHERE_BOUNCING;
					saw->stopAnywhereFlags &= ~ITU_STOPANYWHERE_BOUNCING_T;
					saw->stopAnywhereFlags &= ~ITU_STOPANYWHERE_BOUNCING_B;
				}
			}
			else
			{
				if (targetY > 0)
				{
					if (STOPANYWHERE_DEBUG_FIXXY)
						printf("[DEBUG] Func: %s [Fix Y from %d to %d]\n", __func__, targetY, 0);

					check_result = true;
					result_value = 0;
				}
				else if ((targetY + child->rect.height) < widget->rect.height)
				{
					check_result = true;
					result_value = (widget->rect.height - child->rect.height);
				}
			}

			if (check_result)
			{
				//
			}

			return result_value;
		}
		else
		{
			printf("[ERROR] Func: %s [%d]\n", __func__, __LINE__);
			return result_value;
		}
	}
	else
	{
		printf("[ERROR] Func: %s [%d]\n", __func__, __LINE__);
		return result_value;
	}
}

void ituStopAnywhereSetXY(ITUStopAnywhere* saw, int x, int y, int line)
{
	if (saw)
	{
		ITUWidget* sawwidget = (ITUWidget*)saw;
		ITUWidget* child = StopAnywhereGetVisibleChild(saw);

		if (child)
		{
			int fixX = ituStopAnywhereFixX(saw, x);
			int fixY = ituStopAnywhereFixY(saw, y);

			ituWidgetSetPosition(child, fixX, fixY);

			if (STOPANYWHERE_DEBUG_SETXY)
				printf("[Debug][StopAnywhere %s][X %d][Y %d] [%d]\n", sawwidget->name, fixX, fixY, line);
		}
	}
}

bool ituStopAnywhereFindLoop(ITCTree* tree)
{
	ITCTree *child = tree->child;

	//ITUWidget* checkwidget = (ITUWidget*)child;
	//if (checkwidget)
	//	printf("widget name %s\n", checkwidget->name);

	for (child = tree->child; child; child = child->sibling)
	{
		ITUWidget* ww = (ITUWidget*)child;
		if (ww)
		{
			if (ww->type == ITU_BUTTON)
			{
				ITUButton* button = (ITUButton*)ww;

				if (button->pressed)
				{
					ituExecActions(ww, button->actions, ITU_EVENT_MOUSELONGPRESS, 0);
					return true;
				}
			}
			else
			{
				if (ituStopAnywhereFindLoop(child))
					return true;
			}
		}
	}

	

	return false;
}

int ituStopAnywhereAlignCal(ITUWidget* widget, ITUWidget* sawchild, bool horizontal)
{
	int startBoundary = 0;
	int endBoundary = ((horizontal) ? (widget->rect.width) : (widget->rect.height));
	int resultFirst = 0;
	int resultLast = 0;
	int result = 0;
	bool debug_printf = false;
	bool checkAvgUnit = false;

	ITCTree *child, *tree = (ITCTree*)sawchild;
	ITUWidget* firstChild = NULL;
	ITUWidget* lastChild = NULL;
	ITUStopAnywhere* saw = (ITUStopAnywhere*)widget;

	if (STOPANYWHERE_ALIGN_AVG_CENTER)
	{
		int baseunit = ituStopAnywhereCheckChildSame(sawchild, horizontal);

		if (baseunit)
		{
			int alignavgshiftsize = ((endBoundary - startBoundary) % baseunit) / 2;

			if ((alignavgshiftsize % 2) == 0)
			{
				startBoundary += alignavgshiftsize;
				endBoundary -= alignavgshiftsize;
			}
		}
	}

	if (tree)
	{
		for (child = tree->child; child; child = child->sibling)
		{
			if (child)
			{
				ITUWidget* item = (ITUWidget*)child;
				int checkValuePos = ((horizontal) ? (item->rect.x + sawchild->rect.x) : (item->rect.y + sawchild->rect.y));
				int checkValueSiz = ((horizontal) ? (item->rect.width) : (item->rect.height));

				if ((checkValuePos < startBoundary) && ((checkValuePos + checkValueSiz) > startBoundary))
				{
					firstChild = item;
					//if ((checkValuePos * -1) <= (checkValueSiz / 2))
					//	resultFirst = checkValuePos * -1;
					//else
					//	resultFirst = (checkValuePos + checkValueSiz) * -1;

					if ((startBoundary - checkValuePos) <= (checkValueSiz / 2))
						resultFirst = startBoundary - checkValuePos;
					else
						resultFirst = ((checkValuePos + checkValueSiz) - startBoundary) * -1;
				}

				if ((checkValuePos < endBoundary) && ((checkValuePos + checkValueSiz) > endBoundary))
				{
					lastChild = item;
					if ((endBoundary - checkValuePos) <= (checkValueSiz / 2))
						resultLast = endBoundary - checkValuePos;
					else
						resultLast = ((checkValuePos + checkValueSiz) - endBoundary) * -1;
				}
			}
		}
	}

	result = ((abs(resultFirst) <= abs(resultLast)) ? (resultFirst) : (resultLast));

	if ((saw->frame == 0) && debug_printf)
		printf("[Align Target Cal] [%d]\n", result);

	return result;
}

void ituStopAnywhereAlignAni(ITUStopAnywhere* saw, ITUWidget* sawchild, int target, bool horizontal)
{
	if (target != 0)
	{
		int ani_pos = (target / STOPANYWHERE_ALIGN_SHIFT_FACTOR);
		bool debug_printf = false;

		if (abs(ani_pos) == 0)
			ani_pos = target;

		if (horizontal)
		{
			if (debug_printf)
				printf("[StopAnywhere doing Align from %d to %d] [frame %d]\n", saw->childX, saw->childX + ani_pos, saw->frame);
			ituWidgetSetX(sawchild, saw->childX + ani_pos);
			saw->childX = ituWidgetGetX(sawchild);
		}
		else
		{
			if (debug_printf)
				printf("[StopAnywhere doing Align from %d to %d] [frame %d]\n", saw->childY, saw->childY + ani_pos, saw->frame);
			ituWidgetSetY(sawchild, saw->childY + ani_pos);
			saw->childY = ituWidgetGetY(sawchild);
		}
	}
}

bool ituStopAnywhereUpdate(ITUWidget* widget, ITUEvent ev, int arg1, int arg2, int arg3)
{
	bool result, horizontal;
	ITUStopAnywhere* saw = (ITUStopAnywhere*)widget;
	ITUWidget* sawchild = NULL;
	assert(saw);

	sawchild = StopAnywhereGetVisibleChild(saw);

	if (sawchild == NULL)
		return false;

	horizontal = (sawchild->rect.width > widget->rect.width);

	result = ituWidgetUpdateImpl(widget, ev, arg1, arg2, arg3);

	if ((ev == ITU_EVENT_LOAD) && (TEST_ALIGN))
	{
		int count = 0;
		bool printDebug = true;
		ITCTree *child, *tree = (ITCTree*)sawchild;

		if (tree)
		{
			for (child = tree->child; child; child = child->sibling)
			{
				if (child)
				{
					count++;
					if (printDebug)
					{
						ITUWidget* item = (ITUWidget*)child;

						if (count == 1)
							printf("================================\n[StopAnywhere] [AlignMode : %s]\n", ((horizontal) ? ("Horizontal") : ("Vectical")));

						printf("[%s] item (%d)(%s) at (%d, %d) [next should be at (%d, %d)]\n", widget->name, count, item->name, item->rect.x, item->rect.y, 
							item->rect.x + ((horizontal) ? (item->rect.width) : (0)),
							item->rect.y + ((horizontal) ? (0) : (item->rect.height)));
					}
				}
			}
		}
	}
	else if (ev == ITU_EVENT_TIMER)
	{
		if (widget->flags & ITU_DRAGGING)
		{
			uint32_t longclock = ituWidgetGetCustomData(saw);

			if (longclock)
			{
				if ((itpGetTickCount() - longclock) >= STOPANYWHERE_MOUSEDOWN_LONGPRESS_DELAY)
				{
					ITCTree *tree = (ITCTree*)sawchild;
					ituStopAnywhereFindLoop(tree);

					ituWidgetSetCustomData(saw, 0); //long press clock reset
				}
			}

			if (saw->timerCycleCount < STOPANYWHERE_TIMER_CYCLE_MAX) //17ms x 14 ~ 250ms
				saw->timerCycleCount++;
			else
			{
				saw->timerCycleCount = 0;
				saw->clock = itpGetTickCount();
				//saw->lastTimerX = ituWidgetGetX(sawchild);
				//saw->lastTimerY = ituWidgetGetY(sawchild);
				saw->lastTimerX = saw->timerX;
				saw->lastTimerY = saw->timerY;
			}

			if (STOPANYWHERE_DRAGGING_PIPE && saw->temp3)
			{
				if (saw->temp1 < saw->temp2)
				{
					int* arr = (int*)ituWidgetGetCustomData(saw);
					if (arr)
					{
						int distX = 0;
						int distY = 0;

						if (saw->temp3 == 1)
						{
							distX = arr[saw->temp1 % STOPANYWHERE_DRAGGING_PIPE_SIZE];
						}
						else
						{
							distY = arr[saw->temp1 % STOPANYWHERE_DRAGGING_PIPE_SIZE];
						}

						ituStopAnywhereSetXY(saw, (saw->childX + distX), (saw->childY + distY), __LINE__);

						saw->childX = ituWidgetGetX(sawchild);
						saw->childY = ituWidgetGetY(sawchild);

						//printf("drag arr temp1[%d] to child[%d] distX[%d]\n", saw->temp1, saw->childX, distX);

						saw->temp1++;
					}
				}
				else
				{
				}
			}
		}
		else if (widget->flags & ITU_UNDRAGGING)
		{
			bool debug_msg = true;

			if (saw->stopAnywhereFlags & ITU_STOPANYWHERE_BOUNCING)
			{
				saw->frame += 2;

				if (saw->bounce == 1) //Horizontal bounce
				{
					int baseunit = ituStopAnywhereCheckChildSame(sawchild, true);
					int alignavgshiftsize = (widget->rect.width % baseunit) / 2;

					if (!STOPANYWHERE_ALIGN_AVG_CENTER && ((alignavgshiftsize % 2) != 0))
						alignavgshiftsize = 0;

					if (saw->frame >= saw->totalframe)
					{
						if (saw->stopAnywhereFlags & ITU_STOPANYWHERE_BOUNCING_L)
						{
							saw->stopAnywhereFlags &= ~ITU_STOPANYWHERE_BOUNCING_L;
							ituWidgetSetX(sawchild, 0 + alignavgshiftsize);
						}
						else if (saw->stopAnywhereFlags & ITU_STOPANYWHERE_BOUNCING_R)
						{
							saw->stopAnywhereFlags &= ~ITU_STOPANYWHERE_BOUNCING_R;
							ituWidgetSetX(sawchild, (widget->rect.width - sawchild->rect.width) - alignavgshiftsize);
						}

						if (debug_msg)
							printf("[StopAnywhere][Undragging][X to %d][Frame %d]\n", sawchild->rect.x, saw->frame);

						saw->frame = 0;
						saw->stopAnywhereFlags &= ~ITU_STOPANYWHERE_BOUNCING;
						widget->flags &= ~ITU_UNDRAGGING;
						ituExecActions(widget, saw->actions, ITU_EVENT_STOPPED, 0);
					}
					else
					{
						int ani_pos = saw->childX;

						if (saw->stopAnywhereFlags & ITU_STOPANYWHERE_BOUNCING_L)
							ani_pos = ((saw->childX - alignavgshiftsize) * (saw->totalframe - saw->frame) / saw->totalframe) + alignavgshiftsize;
						else if (saw->stopAnywhereFlags & ITU_STOPANYWHERE_BOUNCING_R)
							ani_pos = saw->childX + ((widget->rect.width - alignavgshiftsize - (saw->childX + sawchild->rect.width)) * saw->frame / saw->totalframe);

						ituWidgetSetX(sawchild, ani_pos);

						if (debug_msg)
							printf("[StopAnywhere][Undragging][X to %d][Frame %d]\n", sawchild->rect.x, saw->frame);
					}
				}
				else if (saw->bounce == 2) //Vertical bounce
				{
					if (saw->frame >= saw->totalframe)
					{
						if (saw->stopAnywhereFlags & ITU_STOPANYWHERE_BOUNCING_T)
						{
							saw->stopAnywhereFlags &= ~ITU_STOPANYWHERE_BOUNCING_T;
							ituWidgetSetY(sawchild, 0);
						}
						else if (saw->stopAnywhereFlags & ITU_STOPANYWHERE_BOUNCING_B)
						{
							saw->stopAnywhereFlags &= ~ITU_STOPANYWHERE_BOUNCING_B;
							ituWidgetSetY(sawchild, (widget->rect.height - sawchild->rect.height));
						}

						if (debug_msg)
							printf("[StopAnywhere][Undragging][Y to %d][Frame %d]\n", sawchild->rect.y, saw->frame);

						saw->frame = 0;
						saw->stopAnywhereFlags &= ~ITU_STOPANYWHERE_BOUNCING;
						widget->flags &= ~ITU_UNDRAGGING;
						ituExecActions(widget, saw->actions, ITU_EVENT_STOPPED, 0);
					}
					else
					{
						int ani_pos = saw->childY;

						if (saw->stopAnywhereFlags & ITU_STOPANYWHERE_BOUNCING_T)
							ani_pos = (saw->childY * (saw->totalframe - saw->frame) / saw->totalframe);
						else if (saw->stopAnywhereFlags & ITU_STOPANYWHERE_BOUNCING_B)
							ani_pos = saw->childY + ((widget->rect.height - (saw->childY + sawchild->rect.height)) * saw->frame / saw->totalframe);

						ituWidgetSetY(sawchild, ani_pos);

						if (debug_msg)
							printf("[StopAnywhere][Undragging][Y to %d][Frame %d]\n", sawchild->rect.y, saw->frame);
					}
				}
			}
			else if (TEST_ALIGN)
			{
				int target = ituStopAnywhereAlignCal(widget, sawchild, horizontal);

				saw->frame++;

				ituStopAnywhereAlignAni(saw, sawchild, target, horizontal);

				if (saw->frame >= saw->totalframe)
				{
					saw->frame = 0;
					widget->flags &= ~ITU_UNDRAGGING;
					ituExecActions(widget, saw->actions, ITU_EVENT_STOPPED, 0);
				}
			}
		}
		else if (saw->stopAnywhereFlags & ITU_STOPANYWHERE_SLIDING)
		{
			int slideX = 0;
			int slideY = 0;

			saw->frame++;
			
			slideX = saw->factorX / (saw->frame + 1);
			slideY = saw->factorY / (saw->frame + 1);

			ituStopAnywhereSetXY(saw, (saw->childX + slideX), (saw->childY + slideY), __LINE__);

			//printf("frame %d, [%d,%d] step dist %d, %d\n", saw->frame, saw->childX, saw->childY, slideX, slideY);

			if ((saw->childX == sawchild->rect.x) && (saw->childY == sawchild->rect.y))
			{
				saw->frame = saw->totalframe;
			}

			saw->childX = ituWidgetGetX(sawchild);
			saw->childY = ituWidgetGetY(sawchild);

			ituExecActions(widget, saw->actions, ITU_EVENT_CHANGED, 0);

			if (saw->frame >= saw->totalframe)
			{
				saw->stopAnywhereFlags &= ~ITU_STOPANYWHERE_SLIDING;
				saw->frame = 0;

				if (saw->stopAnywhereFlags & ITU_STOPANYWHERE_BOUNCING)
				{
					widget->flags |= ITU_UNDRAGGING;
				}
				else if (TEST_ALIGN)
				{
					widget->flags |= ITU_UNDRAGGING;
				}
				else
					ituExecActions(widget, saw->actions, ITU_EVENT_STOPPED, 0);
			}
		}
	}
	else if (ev == ITU_EVENT_MOUSEDOWN)
	{
		if (saw->stopAnywhereFlags & ITU_STOPANYWHERE_SLIDING)
		{
			saw->stopAnywhereFlags &= ~ITU_STOPANYWHERE_SLIDING;
			saw->frame = 0;
			saw->factorX = 0;
			saw->factorY = 0;

			ituUnPressWidget(sawchild);
		}

		if (widget->flags & ITU_DRAGGABLE)
		{
			int x = arg2 - widget->rect.x;
			int y = arg3 - widget->rect.y;

			if (saw->stopAnywhereFlags & ITU_STOPANYWHERE_BOUNCING)
			{
				ituUnPressWidget(sawchild);
			}
			else if ((ituWidgetIsInside(widget, x, y)) && (ituScene->dragged == NULL))
			{
				uint32_t longclock = itpGetTickCount();
				ituWidgetSetCustomData(saw, longclock); //long press clock init

				saw->initX = saw->touchX = x;
				saw->initY = saw->touchY = y;
				//saw->lastTimerX = saw->childX = ituWidgetGetX(sawchild);
				//saw->lastTimerY = saw->childY = ituWidgetGetY(sawchild);

				saw->timerX = saw->lastTimerX = x;
				saw->timerY = saw->lastTimerY = y;
				saw->childX = ituWidgetGetX(sawchild);
				saw->childY = ituWidgetGetY(sawchild);

				saw->timerCycleCount = 0;
				saw->clock = itpGetTickCount();

				widget->flags |= ITU_DRAGGING;
				ituScene->dragged = widget;

				if (STOPANYWHERE_DRAGGING_PIPE && saw->temp3)
				{
					int* arr = (int*)ituWidgetGetCustomData(saw);
					if (arr)
					{
						int i;
						for (i = 0; i < STOPANYWHERE_DRAGGING_PIPE_SIZE; i++)
						{
							arr[i] = -1;
						}
						saw->temp5 = ((saw->temp3 == 1) ? (x) : (y));
						saw->temp1 = 0;
						saw->temp2 = 0;
					}
				}
			}
		}
	}
	else if (ev == ITU_EVENT_MOUSEUP)
	{
		if (widget->flags & ITU_DRAGGING)
		{
			uint32_t timetick = itpGetTickCount();

			if ((timetick - saw->clock) <= STOPANYWHERE_SLIDE_CHECK_TIMING)
			{
				//int valueX = ituWidgetGetX(sawchild) - saw->lastTimerX;
				//int valueY = ituWidgetGetY(sawchild) - saw->lastTimerY;
				int valueX = saw->timerX - saw->lastTimerX;
				int valueY = saw->timerY - saw->lastTimerY;

				int lastDistX = abs(valueX);
				int lastDistY = abs(valueY);
				int lastDist = (lastDistX * lastDistX) + (lastDistY * lastDistY);

				if (lastDist > STOPANYWHERE_SLIDE_REQUIRE_DIST)
				{
					saw->factorX = valueX;
					saw->factorY = valueY;

					saw->frame = 0;
					saw->stopAnywhereFlags |= ITU_STOPANYWHERE_SLIDING;

					if (STOPANYWHERE_DRAGGING_PIPE && saw->temp3)
					{
						//ituStopAnywhereSetXY(saw, (saw->childX + distX), (saw->childY + distY), __LINE__);
						saw->temp1 = 0;
						saw->temp2 = 0;
					}


					printf("Sliding factorX %d, factorY %d\n", saw->factorX, saw->factorY);
				}
				else
				{
					printf("[1800][lastDist, lastDistX, lastDistY] [%d, %d, %d]\n", lastDist, lastDistX, lastDistY);
				}
			}
			else
				printf("[%d][Clock][Tick, saw_last_clock][%d, %d]\n", STOPANYWHERE_SLIDE_CHECK_TIMING, timetick, saw->clock);

			saw->touchX = 0;
			saw->touchY = 0;
			saw->childX = ituWidgetGetX(sawchild);
			saw->childY = ituWidgetGetY(sawchild);

			widget->flags &= ~ITU_DRAGGING;
			ituScene->dragged = NULL;

			if (!(saw->stopAnywhereFlags & ITU_STOPANYWHERE_SLIDING))
			{
				if ((saw->stopAnywhereFlags & ITU_STOPANYWHERE_BOUNCING) || (TEST_ALIGN))
					widget->flags |= ITU_UNDRAGGING;
				else
					ituExecActions(widget, saw->actions, ITU_EVENT_STOPPED, 0);

				saw->frame = 0;
				ituWidgetSetDirty(widget, true);
			}
		}
		else if (saw->stopAnywhereFlags & ITU_STOPANYWHERE_BOUNCING)
		{
			ituUnPressWidget(sawchild);
		}
	}
	else if (ev == ITU_EVENT_MOUSEMOVE)
	{
		if (widget->flags & ITU_DRAGGING)
		{
			int x = arg2 - widget->rect.x;
			int y = arg3 - widget->rect.y;
			int distX = x - saw->touchX;
			int distY = y - saw->touchY;
			int Xtd = x - saw->initX;
			int Ytd = y - saw->initY;

			saw->timerX = x;
			saw->timerY = y;

			if ((abs(Xtd) >= ITU_DRAG_DISTANCE) || (abs(Ytd) >= ITU_DRAG_DISTANCE))
			{
				ituWidgetSetCustomData(saw, 0); //long press clock init

				ituUnPressWidget(sawchild);
				//printf("[Unpress] x,y[%d,%d] init[%d,%d]\n", x, y, saw->initX, saw->initY);
			}

			if (ituWidgetIsInside(widget, x, y))
			{
				//ituStopAnywhereSetXY(saw, (saw->childX + distX), (saw->childY + distY), __LINE__);
				saw->touchX = x;
				saw->touchY = y;
				saw->childX = ituWidgetGetX(sawchild);
				saw->childY = ituWidgetGetY(sawchild);

				ituExecActions(widget, saw->actions, ITU_EVENT_CHANGED, 0);

				if (STOPANYWHERE_DRAGGING_PIPE && saw->temp3)
				{
					int* arr = (int*)ituWidgetGetCustomData(saw);
					if (arr)
					{
						int i = saw->temp5;
						int target = ((saw->temp3 == 1) ? (x) : (y));
						int count = 0;
						int avg = abs(i - target) / 3;
						int mode = ((i < target) ? (0) : (1));
						bool working = true;

						if (avg == 0)
							avg = 1;

						while (working)
						{
							//int ddd;
							int step = ((i < target) ? (avg) : (-avg));
							i += step;

							if ((i >= target) && (mode == 0))
							{
								if (i > target)
									step -= (i - target);
								working = false;
							}
							else if ((i <= target) && (mode == 1))
							{
								if (i < target)
									step += (target - i);
								working = false;
							}

							arr[saw->temp2 % STOPANYWHERE_DRAGGING_PIPE_SIZE] = step; //i - saw->temp5;
							
							//ddd = arr[saw->temp2 % STOPANYWHERE_DRAGGING_PIPE_SIZE];
							//ddd++;

							saw->temp2++;
							count++;
						}

						saw->temp5 = target;
						//ituWidgetSetCustomData(saw, (void*)arr);
						//printf("drag arr item %d arranged.\n", count);
					}
				}
				else
				{
					ituStopAnywhereSetXY(saw, (saw->childX + distX), (saw->childY + distY), __LINE__);
				}
			}
		}
	}

	result |= widget->dirty;

	//if (result)
	//	printf("[StopAnywhere][%s][Update True].\n", widget->name);

	return widget->visible ? result : false;
}

void ituStopAnywhereDraw(ITUWidget* widget, ITUSurface* dest, int x, int y, uint8_t alpha)
{
	int destx, desty;
	uint8_t desta;
	ITUStopAnywhere* saw = (ITUStopAnywhere*)widget;
	ITURectangle* rect = &widget->rect;
	ITCTree* node;
	ITURectangle prevClip;
	assert(saw);
	assert(dest);

	if (!widget->visible)
		return;

	destx = rect->x + x;
	desty = rect->y + y;
	desta = alpha * widget->alpha / 255;

	if (widget->angle == 0)
	{
		for (node = widget->tree.child; node; node = node->sibling)
		{
			ITUWidget* child = (ITUWidget*)node;

			if (child == NULL)
				continue;

			//if ((widget->flags & ITU_DRAGGING) && (child != ituScene->dragged))
			ituWidgetSetClipping(widget, dest, x, y, &prevClip);

			if (child->visible && ituWidgetIsOverlapClipping(child, dest, destx, desty))
			{
				bool motiondebug = false;
				bool movecheck = false;

				if ((abs(saw->temp1 - saw->childX) >= 2) || (abs(saw->temp2 - saw->childY) >= 2))
					movecheck = true;

				if (STOPANYWHERE_DRAGGING_PIPE)
					movecheck = false;

				if ((widget->flags & ITU_DRAGGING) && (STOPANYWHERE_DRAGGING_EYEMOTION) && movecheck)
				{
					int x_step = saw->temp1 - saw->childX;
					int y_step = saw->temp2 - saw->childY;

					

					if (STOPANYWHERE_DRAGGING_EYEMOTION == 2)
					{
						ituWidgetDraw(node, dest, destx + (x_step / 2), desty + (y_step / 2), desta / 2);
						if (motiondebug)
							printf("[StopAnywhere]eye_motion[1][%d,%d]\n\n", destx + (x_step / 2), desty + (y_step / 2));

						ituWidgetDraw(node, dest, destx + x_step, desty + y_step, desta / 4);
						if (motiondebug)
							printf("[StopAnywhere]eye_motion[2][%d,%d]\n", destx + x_step, desty + y_step);

						
					}
					else if (STOPANYWHERE_DRAGGING_EYEMOTION == 1)
					{
						/*ituWidgetDraw(node, dest, destx + x_step, desty + y_step, desta / 4);
						if (motiondebug)
							printf("[StopAnywhere]eye_motion[1][%d,%d]\n", destx + x_step, desty + y_step);*/

						ituWidgetDraw(node, dest, destx + (x_step / 2), desty + (y_step / 2), desta / 4);
						if (motiondebug)
							printf("[StopAnywhere]eye_motion[1][%d,%d]\n\n", destx + (x_step / 2), desty + (y_step / 2));
					}

					ituWidgetDraw(node, dest, destx, desty, desta);

					saw->temp1 = saw->childX;
					saw->temp2 = saw->childY;
				}
				else
					ituWidgetDraw(node, dest, destx, desty, desta);

				child->dirty = false;
			}

			//if ((widget->flags & ITU_DRAGGING) && (child != ituScene->dragged))
			ituSurfaceSetClipping(dest, prevClip.x, prevClip.y, prevClip.width, prevClip.height);
		}
	}
	else
	{
		ITUSurface* surf;

		surf = ituCreateSurface(rect->width, rect->height, 0, ITU_ARGB8888, NULL, 0);
		if (surf)
		{
			ITUColor color = { 0, 0, 0, 0 };

			ituColorFill(surf, 0, 0, rect->width, rect->height, &color);

			for (node = widget->tree.child; node; node = node->sibling)
			{
				ITUWidget* child = (ITUWidget*)node;

				if (child->visible && ituWidgetIsOverlapClipping(child, dest, destx, desty))
					ituWidgetDraw(node, surf, 0, 0, desta);

				child->dirty = false;
			}

#ifdef CFG_WIN32_SIMULATOR
			ituRotate(dest, destx + rect->width / 2, desty + rect->height / 2, surf, surf->width / 2, surf->height / 2, (float)widget->angle, 1.0f, 1.0f);
#else
			ituRotate(dest, destx, desty, surf, surf->width / 2, surf->height / 2, (float)widget->angle, 1.0f, 1.0f);
#endif
			ituDestroySurface(surf);
		}
	}

	ituDirtyWidget(widget, false);
}

void ituStopAnywhereOnAction(ITUWidget* widget, ITUActionType action, char* param)
{
	ITUStopAnywhere* saw = (ITUStopAnywhere*)widget;
	assert(saw);

	switch (action)
	{
	case ITU_ACTION_RELOAD:
		ituStopAnywhereUpdate(widget, ITU_EVENT_LAYOUT, 0, 0, 0);
		break;

	default:
		ituWidgetOnActionImpl(widget, action, param);
		break;
	}
}

void ituStopAnywhereInit(ITUStopAnywhere* saw)
{
    assert(saw);
    ITU_ASSERT_THREAD();

    memset(saw, 0, sizeof (ITUStopAnywhere));

	ituWidgetInit(&saw->widget);
    
    ituWidgetSetType(saw, ITU_STOPANYWHERE);
    ituWidgetSetName(saw, sawName);
	ituWidgetSetUpdate(saw, ituStopAnywhereUpdate);
	ituWidgetSetDraw(saw, ituStopAnywhereDraw);
	ituWidgetSetOnAction(saw, ituStopAnywhereOnAction);
}

void ituStopAnywhereLoad(ITUStopAnywhere* saw, uint32_t base)
{
    assert(saw);

	//saw->bounce = 2;
	ituWidgetLoad(&saw->widget, base);
	ituWidgetSetUpdate(saw, ituStopAnywhereUpdate);
	ituWidgetSetDraw(saw, ituStopAnywhereDraw);
	ituWidgetSetOnAction(saw, ituStopAnywhereOnAction);

	if (STOPANYWHERE_DRAGGING_PIPE)
	{
		int* check_custom = (int*)ituWidgetGetCustomData(saw);
		ITUWidget* widget = (ITUWidget*)saw;
		ITUWidget* sawchild = StopAnywhereGetVisibleChild(saw);
		saw->temp3 = 0;

		if (((sawchild->rect.width - ituWidgetGetWidth(saw)) > 100) && (sawchild->rect.height <= ituWidgetGetHeight(saw)))
			saw->temp3 = 1; //Horizontal
		else if (((sawchild->rect.height - ituWidgetGetHeight(saw)) > 100) && (sawchild->rect.width <= ituWidgetGetWidth(saw)))
			saw->temp3 = 2; //Vertical

		if ((check_custom == NULL) && saw->temp3)
		{
			int* drag_arr = (int*)malloc(sizeof(int)* 1000);
			memset(drag_arr, -1, sizeof(int)* 1000);
			saw->temp1 = 0;
			saw->temp2 = 0;
			ituWidgetSetCustomData(saw, (void*)drag_arr);
		}
	}

	
}

int ituStopAnywhereGetChildX(ITUStopAnywhere* saw)
{
	assert(saw);

	return saw->childX;
}

int ituStopAnywhereGetChildY(ITUStopAnywhere* saw)
{
	assert(saw);

	return saw->childY;
}

void ituStopAnywhereResetXY(ITUStopAnywhere* saw)
{
	assert(saw);

	ituStopAnywhereSetXY(saw, 0, 0, __LINE__);
}