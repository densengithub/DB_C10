#include "ite/itu.h"

//Logo
extern bool LogoOnEnter(ITUWidget *widget, char *param);
extern bool LogoOnTimer(ITUWidget* widget, char* param);

//Main
extern bool MainOnEnter(ITUWidget* widget, char* param);
extern bool MainOnTimer(ITUWidget* widget, char* param);
//extern bool MainButtonOnPress(ITUWidget* widget, char* param);

//Upgrade
extern bool UpgradeOnEnter(ITUWidget* widget, char* param);
extern bool UpgradeOnTimer(ITUWidget* widget, char* param);

//Charge
extern bool ChargeOnEnter(ITUWidget* widget, char* param);
extern bool ChargeOnTimer(ITUWidget* widget, char* param);

//TestMode
extern bool TestModeOnEnter(ITUWidget* widget, char* param);
extern bool TestModeOnTimer(ITUWidget* widget, char* param);

ITUActionFunction actionFunctions[] =
{
        // Logo
        "LogoOnEnter", LogoOnEnter,
        // "LogoOnTimer", LogoOnTimer,

        // Main
        "MainOnEnter", MainOnEnter,
        "MainOnTimer", MainOnTimer,

        // Charge
        "ChargeOnEnter", ChargeOnEnter,
        "ChargeOnTimer", ChargeOnTimer,

        // Upgrade
        "UpgradeOnEnter", UpgradeOnEnter,
        "UpgradeOnTimer", UpgradeOnTimer,

        // TestMode
        "TestModeOnEnter", TestModeOnEnter,
        "TestModeOnTimer", TestModeOnTimer,

        NULL, NULL
};
