#include <assert.h>
#include <sys/time.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SDL/SDL.h"
#include "scene.h"
#include "stdint.h"
#include "project.h"

/* widgets:
testmodeLayer
Container4
testPassContainer
testPassText
testPassSprite
Icon659
Icon660
testRow6Container
testLedAbsContainer
testLedAbsSprite
Icon754
Icon755
Text752
testLedLeftContainer
testLedLeftSprite
Icon759
Icon760
Text757
testLedLowContainer
testLedLowSprite
Icon764
Icon765
Text762
testLedFarContainer
testLedFarSprite
Icon769
Icon770
Text767
testLedChgContainer
testLedChgSprite
Icon774
Icon775
Text772
testLedParkContainer
testLedParkSprite
Icon779
Icon780
Text777
testLedFaultContainer
testLedFaultSprite
Icon784
Icon785
Text782
testLedWarnContainer
testLedWarnSprite
Icon789
Icon790
Text787
testLedHotContainer
testLedHotSprite
Icon794
Icon795
Text792
testLedRightContainer
testLedRightSprite
Icon799
Icon800
Text797
testLedSysContainer
testLedSysSprite
Icon804
Icon805
Text802
testLedText1
Text807
Icon808
testRow5Container
testInAccContainer
testInAccSprite
Icon812
Icon813
Text810
testInBp2Container
testInBp2Sprite
Icon726
Icon727
Text724
testInBp1Container
testInBp1Sprite
Icon731
Icon732
Text729
testInLightContainer
testInLightSprite
Icon736
Icon737
Text734
testInLeftContainer
testInLeftSprite
Icon741
Icon742
Text739
testInRightContainer
testInRightSprite
Icon746
Icon747
Text744
testInputText1
Text749
Icon750
testRow4Container
Container56
Container64
testBtSprite
Icon66
Icon67
Icon68
Text69
Icon70
Container29
Container57
testSpiflashSprite
Icon59
Icon60
Icon61
Text62
Icon63
testRow3Container
Container39
Container49
testRs485Sprite
Icon51
Icon52
Icon53
Text54
Icon55
Container28
Container40
testNtcSprite
Icon42
Icon43
Icon44
testNtcText2
testNtcText1
Text47
Icon48
testRow2Container
testPsensor2Container
Container2
testPsensor2Sprite
Icon4
Icon5
Icon6
testPsensor2Text
Text8
Icon9
testPsensor1Container
Container13
testPsensor1Sprite
Icon15
Icon16
Icon17
testPsensor1Text
Text19
Icon20
testRow1Container
Container12
Container32
testUsbSprite
Icon34
Icon35
Icon36
Text37
Icon38
Container11
Container21
testUartSprite
Icon23
Icon24
Icon25
Text26
Icon27
Background227
Background28
Background19
*/
#pragma execution_character_set("utf-8")
#define  LED_MAX        11
#define  INPUT_MAX      6

//testmodeLayer
ITULayer* testmodeLayer = NULL;
static ITUSprite* testUartSprite = NULL;
static ITUSprite* testUsbSprite = NULL;
static ITUSprite* testPsensor1Sprite = NULL;
static ITUSprite* testPsensor2Sprite = NULL;
static ITUSprite* testNtcSprite = NULL;
static ITUSprite* testRs485Sprite = NULL;
static ITUSprite* testSpiflashSprite = NULL;
static ITUSprite* testBtSprite = NULL;

static ITUSprite* testPassSprite = NULL;


// static ITUContainer* testLedSysContainer = NULL;
// static ITUContainer* testLedRightContainer = NULL;
// static ITUContainer* testLedHotContainer = NULL;
// static ITUContainer* testLedWarnContainer = NULL;
// static ITUContainer* testLedFaultContainer = NULL;
// static ITUContainer* testLedParkContainer = NULL;
// static ITUContainer* testLedChgContainer = NULL;
// static ITUContainer* testLedFarContainer = NULL;
// static ITUContainer* testLedLowContainer = NULL;
// static ITUContainer* testLedLeftContainer = NULL;
// static ITUContainer* testLedAbsContainer = NULL;
static ITUSprite* testLedContainer[LED_MAX] = {NULL};

// static ITUSprite* testLedSysSprite = NULL;
// static ITUSprite* testLedRightSprite = NULL;
// static ITUSprite* testLedHotSprite = NULL;
// static ITUSprite* testLedWarnSprite = NULL;
// static ITUSprite* testLedFaultSprite = NULL;
// static ITUSprite* testLedParkSprite = NULL;
// static ITUSprite* testLedChgSprite = NULL;
// static ITUSprite* testLedFarSprite = NULL;
// static ITUSprite* testLedLowSprite = NULL;
// static ITUSprite* testLedLeftSprite = NULL;
// static ITUSprite* testLedAbsSprite = NULL;
static ITUSprite* testLedSprite[LED_MAX] = {NULL};

// static ITUContainer* testInRightContainer = NULL;
// static ITUContainer* testInLeftContainer = NULL;
// static ITUContainer* testInLightContainer = NULL;
// static ITUContainer* testInBp1Container = NULL;
// static ITUContainer* testInBp2Container = NULL;
// static ITUContainer* testInAccContainer = NULL;
static ITUContainer* testInputContainer[INPUT_MAX] = {NULL};

// static ITUSprite* testInRightSprite = NULL;
// static ITUSprite* testInLeftSprite = NULL;
// static ITUSprite* testInLightSprite = NULL;
// static ITUSprite* testInBp1Sprite = NULL;
// static ITUSprite* testInBp2Sprite = NULL;
// static ITUSprite* testInAccSprite = NULL;
static ITUSprite* testInputSprite[INPUT_MAX] = {NULL};

// static ITUContainer* testUartContainer = NULL;
// static ITUContainer* testUsbContainer = NULL;
static ITUContainer* testPsensor1Container = NULL;
static ITUContainer* testPsensor2Container = NULL;
static ITUContainer* testNtcContainer = NULL;
static ITUContainer* testRs485Container = NULL;
static ITUContainer* testSpiflashContainer = NULL;
static ITUContainer* testBtContainer = NULL;
// static ITUContainer* testInputContainer = NULL;
// static ITUContainer* testLedContainer = NULL;

static ITUContainer* testRow1Container = NULL;
static ITUContainer* testRow2Container = NULL;
static ITUContainer* testRow3Container = NULL;
static ITUContainer* testRow4Container = NULL;
static ITUContainer* testRow5Container = NULL;
static ITUContainer* testRow6Container = NULL;

static ITUContainer* testPassContainer = NULL;

static ITUText* testPsensor1Text = NULL;
static ITUText* testPsensor2Text = NULL;
static ITUText* testNtcText1 = NULL;
static ITUText* testNtcText2 = NULL;
static ITUText* testInputText1 = NULL;
static ITUText* testLedText1 = NULL;

static ITUText* testText1 = NULL;
static ITUText* testText2 = NULL;

static ITUText* testPassText = NULL;

static char tempbuff[10] = {0};

bool testmodeLayer_module_Display(bool dis_visible,int dis_list)
{
    switch(dis_list)
    {
        case 0:
            ituWidgetSetVisible(testPsensor2Container, dis_visible);
            ituWidgetSetVisible(testRow2Container, true);
            
            ituWidgetSetVisible(testRow3Container, dis_visible);
            ituWidgetSetVisible(testRow4Container, dis_visible);
            ituWidgetSetVisible(testRow5Container, dis_visible);
            ituWidgetSetVisible(testRow6Container, dis_visible);
            break;
        case 1:
            break;
        case 2:
            break;
        case 3:
            ituWidgetSetVisible(testPsensor1Container, dis_visible);
            break;
        case 4:
            ituWidgetSetVisible(testPsensor2Container, dis_visible); 
            break;   
        case 5:
            ituWidgetSetVisible(testRow3Container, true); 
            ituWidgetSetVisible(testNtcContainer, dis_visible); 
            break;   
        case 6:
            ituWidgetSetVisible(testRow3Container, true); 
            ituWidgetSetVisible(testRs485Container, dis_visible); 
            break;   
        case 7:
            ituWidgetSetVisible(testRow4Container, true); 
            ituWidgetSetVisible(testSpiflashContainer, dis_visible); 
            break;  
        case 8:
            ituWidgetSetVisible(testRow4Container, true);  
            ituWidgetSetVisible(testBtContainer, dis_visible); 
            break;   
        case 9:
            ituWidgetSetVisible(testRow5Container, dis_visible);            
            break; 
        case 10:            
            ituWidgetSetVisible(testRow6Container, dis_visible);
            break;   
        default:
            break;                               
    }
    
    return true;
}


bool testmodeLayer_usb_DisplayConfig(int usb_dis)
{
    if (usb_dis < 3)
    {
        ituSpriteGoto(testUsbSprite, usb_dis);
    }
    return true;
}

bool testmodeLayer_uart_DisplayConfig(int uart_dis)
{
    if (uart_dis < 3)
    {
        ituSpriteGoto(testUartSprite, uart_dis);
    }
    return true;
}

bool testmodeLayer_rs485_DisplayConfig(int rs485_dis)
{
    if (rs485_dis < 3)
    {
        ituSpriteGoto(testRs485Sprite, rs485_dis);
        ituWidgetSetVisible(testRow3Container, true);
    }  

    return true;
}

bool testmodeLayer_bluetooth_DisplayConfig(int bt_dis)
{
    if (bt_dis < 3)
    {
        ituSpriteGoto(testBtSprite, bt_dis);
        ituWidgetSetVisible(testRow4Container, true);
    }
    return true;
}

bool testmodeLayer_spiflash_DisplayConfig(int flash_dis)
{
    if (flash_dis < 3)
    {
        ituSpriteGoto(testSpiflashSprite, flash_dis);
        ituWidgetSetVisible(testRow4Container, true);
    }
    return true;
}

// bool testmodeLayer_ntc_DisplayConfig(int ntc_ad_dis,int ntc_dat_dis,int ntc_dis)
bool testmodeLayer_ntc_DisplayConfig(int ntc_ad_dis,int ntc_dat_dis)
{
    char buff[10] = {0};
    char buff1[20] = {0};
    bool ret = false;
    // if (ntc_dis < 3)
    // {
        // ituSpriteGoto(testNtcSprite, ntc_dis);
        if (ntc_ad_dis >= 0x0FFF || ntc_ad_dis == 0)
        {
            ituSpriteGoto(testNtcSprite, 1);
        }
        else
        {
            ituSpriteGoto(testNtcSprite, 2);
            ret = true;
        }
    // }
   
    sprintf(buff, "0x%04x", ntc_ad_dis);
    ituTextSetString(testNtcText1, buff);

    // printf("totalSize = %d....\t\n", testNtcText2->stringSet->totalSize);
    // memcpy(buff, testNtcText2->stringSet->strings[0], 7);
    // strcpy(buff, testNtcText2->stringSet->strings[0]);
    // printf("testNtcText2 = %s....\t\n", testNtcText2->stringSet->strings[0]);
    // printf("buff= %s....\t\n", buff);
    sprintf(buff1, "%d", ntc_dat_dis);
    // strcat(buff1, &tempbuff[3]);
    strcat(buff1, (testNtcText2->stringSet->strings[0]+3));
    // printf("buff1 = %s....\t\n", buff1);
    ituTextSetString(testNtcText2, buff1);    
    ituWidgetSetVisible(testRow3Container, true);

    return ret;
}


// bool testmodeLayer_psensor1_DisplayConfig(int psensor_ad_dis,int psensor_dis)
bool testmodeLayer_psensor1_DisplayConfig(int psensor_ad_dis)
{
    char buff[10] = {0};
    bool ret = false;
    // if (psensor_dis < 3)
    // {
        if (psensor_ad_dis >= 0x0FFF || psensor_ad_dis == 0)
        {
            ituSpriteGoto(testPsensor1Sprite, 1);
        }
        else
        {
            ituSpriteGoto(testPsensor1Sprite, 2);
            ret = true;
        }
    // }
    sprintf(buff, "0x%04x", psensor_ad_dis);
    ituTextSetString(testPsensor1Text, buff);   
    return ret;
}

// bool testmodeLayer_psensor2_DisplayConfig(int psensor_ad_dis,int psensor_dis)
bool testmodeLayer_psensor2_DisplayConfig(int psensor_ad_dis)
{
    char buff[10] = {0};
    bool ret = false;
    // if (psensor_dis < 3)
    // {
        if (psensor_ad_dis >= 0x0FFF || psensor_ad_dis == 0)
        {
            ituSpriteGoto(testPsensor2Sprite, 1);
        }
        else
        {
            ituSpriteGoto(testPsensor2Sprite, 2);
            ret = true;
        }
    // }   
    
    sprintf(buff, "0x%04x", psensor_ad_dis);
    ituTextSetString(testPsensor2Text, buff);
    ituWidgetSetVisible(testPsensor2Container, true);
    return ret;
}

void Layer_Sprite_Init(ITUSprite *sprite, int dly, int idx)
{
    if(sprite != NULL)
    {
        sprite->delay = dly;
		ituSpriteStop(sprite);
		ituSpriteGoto(sprite, idx);
    }
}

bool testmodeLayer_input_DisplayConfig(int input_dis)
{
    int i = 0, dis_input = input_dis;
    int dis_index = 0;
    char buff[10] = {0};
    sprintf(buff, "0x%02x", dis_input);
    ituTextSetString(testInputText1, buff);
    for (i = 0; i < INPUT_MAX; i++)
    {
        if (dis_input & 0x01)
        {
            dis_index = 1;
        }
        else
        {
            dis_index = 0;
        }
        ituWidgetSetVisible(testInputContainer[i], true);
        Layer_Sprite_Init(testInputSprite[i], dis_index);
        dis_input >>= 1;
    }
    ituWidgetSetVisible(testRow5Container, true);
    return true;
}


bool testmodeLayer_led_DisplayConfig(int led_dis)
{
    int i = 0, dis_led = led_dis;
    int dis_index = 0;
    char buff[10] = {0};
    sprintf(buff, "0x%04x", dis_led);
    ituTextSetString(testLedText1, buff);
    for (i = 0; i < LED_MAX; i++)
    {
        if (dis_led & 0x01)
        {
            dis_index = 1;
        }
        else
        {
            dis_index = 0;
        }
        ituWidgetSetVisible(testLedContainer[i], true);
        Layer_Sprite_Init(testLedSprite[i], dis_index);
        dis_led >>= 1;
    }
    ituWidgetSetVisible(testRow6Container, true);
    return true;
}

bool testmodeLayer_pass_DisplayConfig(int pass_dis)
{
    bool isvisible = false;
    char buff[8] = {0};
    switch (pass_dis)
    {
    case 0:
        buff[0] = 'F';
        buff[1] = 'A';
        buff[2] = 'I';
        buff[3] = 'L';
        isvisible = true;
        
        break;
    case 1:
        buff[0] = 'P';
        buff[1] = 'A';
        buff[2] = 'S';
        buff[3] = 'S';
        isvisible = true;
        break;
    default:
        isvisible = false;
        break;
    }
      
    if(isvisible)
    {       
        ituTextSetString(testPassText, buff);
        ituSpriteGoto(testPassSprite, pass_dis);  
    }
    ituWidgetSetVisible(testPassContainer,isvisible);

    return true;
}


void Layer_Uart_Init(void)
{
    if(!testUartSprite)
    {
        testUartSprite = ituSceneFindWidget(&theScene, "testUartSprite");
        assert(testUartSprite);
        Layer_Sprite_Init(testUartSprite,0);
    }
}

void Layer_Usb_Init(void)
{
    if(!testUsbSprite)
    {
        testUsbSprite = ituSceneFindWidget(&theScene, "testUsbSprite");
        assert(testUsbSprite);
        Layer_Sprite_Init(testUsbSprite,0);	
    }
}

void Layer_Psensor1_Init(void)
{
    if(!testPsensor1Sprite)
    {
        testPsensor1Sprite = ituSceneFindWidget(&theScene, "testPsensor1Sprite");
        assert(testPsensor1Sprite);
        testPsensor1Text = ituSceneFindWidget(&theScene, "testPsensor1Text");
        assert(testPsensor1Text);
        testPsensor1Container = ituSceneFindWidget(&theScene, "testPsensor1Container");
        assert(testPsensor1Container);
        Layer_Sprite_Init(testPsensor1Sprite,0);
        ituTextSetString(testPsensor1Text, "0x0000");
    }
}

void Layer_Psensor2_Init(void)
{
    if(!testPsensor2Sprite)
    {
        testPsensor2Sprite = ituSceneFindWidget(&theScene, "testPsensor2Sprite");
        assert(testPsensor2Sprite);
        testPsensor2Text = ituSceneFindWidget(&theScene, "testPsensor2Text");
        assert(testPsensor2Text);
        testPsensor2Container = ituSceneFindWidget(&theScene, "testPsensor2Container");
        assert(testPsensor2Container);

        ituWidgetSetVisible(testPsensor2Container, false);

        Layer_Sprite_Init(testPsensor2Sprite, 0);
        ituTextSetString(testPsensor2Text, "0x0000");
    }
}

void Layer_Ntc_Init(void)
{
    if(!testNtcSprite)
    {
        testNtcSprite = ituSceneFindWidget(&theScene, "testNtcSprite");
        assert(testNtcSprite);
        testNtcText1 = ituSceneFindWidget(&theScene, "testNtcText1");
        assert(testNtcText1);
        testNtcText2 = ituSceneFindWidget(&theScene, "testNtcText2");
        assert(testNtcText2);
        testNtcContainer = ituSceneFindWidget(&theScene, "testNtcContainer");
        assert(testNtcContainer);

        Layer_Sprite_Init(testNtcSprite, 0);
        ituTextSetString(testNtcText1, "0x0000");
        // printf("totalSize = %d....\t\n", testNtcText2->stringSet->totalSize);
        // ituTextSetString(testNtcText2, "---°C");
        strcpy(tempbuff, testNtcText2->stringSet->strings[0]);
    }
}

void Layer_Rs485_Init(void)
{
    if(!testRs485Sprite)
    {
        testRs485Sprite = ituSceneFindWidget(&theScene, "testRs485Sprite");
        assert(testRs485Sprite);
        testRs485Container = ituSceneFindWidget(&theScene, "testRs485Container");
        assert(testRs485Container);
        Layer_Sprite_Init(testRs485Sprite, 0);
    }
}

void Layer_Spiflash_Init(void)
{
    if(!testSpiflashSprite)
    {
        testSpiflashSprite = ituSceneFindWidget(&theScene, "testSpiflashSprite");
        assert(testSpiflashSprite);
        testSpiflashContainer = ituSceneFindWidget(&theScene, "testSpiflashContainer");
        assert(testSpiflashContainer);
        Layer_Sprite_Init(testSpiflashSprite, 0);
    }
}

void Layer_Bt_Init(void)
{
    if(!testBtSprite)
    {
        testBtSprite = ituSceneFindWidget(&theScene, "testBtSprite");
        assert(testBtSprite);
        testBtContainer = ituSceneFindWidget(&theScene, "testBtContainer");
        assert(testBtContainer);
        Layer_Sprite_Init(testBtSprite, 0);
    }
}

void Layer_Input_Init(void)
{
    int i = 0;
    // static ITUSprite* testInRightSprite = NULL;
    // static ITUSprite* testInLeftSprite = NULL;
    // static ITUSprite* testInLightSprite = NULL;
    // static ITUSprite* testInBp1Sprite = NULL;
    // static ITUSprite* testInBp2Sprite = NULL;
    // static ITUSprite* testInAccSprite = NULL;

    // static ITUContainer *testInRightContainer = NULL;
    // static ITUContainer *testInLeftContainer = NULL;
    // static ITUContainer *testInLightContainer = NULL;
    // static ITUContainer *testInBp1Container = NULL;
    // static ITUContainer *testInBp2Container = NULL;
    // static ITUContainer *testInAccContainer = NULL;

    if(!testInputText1)
    {
        testInputText1 = ituSceneFindWidget(&theScene, "testInputText1");
        assert(testInputText1);  

        //    testInputSprite
        testInputSprite[0] = ituSceneFindWidget(&theScene, "testInRightSprite");
        assert(testInputSprite[0]);  
        testInputSprite[1] = ituSceneFindWidget(&theScene, "testInLeftSprite");
        assert(testInputSprite[1]);  
        testInputSprite[2] = ituSceneFindWidget(&theScene, "testInLightSprite");
        assert(testInputSprite[2]);  
        testInputSprite[3] = ituSceneFindWidget(&theScene, "testInBp1Sprite");
        assert(testInputSprite[3]);  
        testInputSprite[4] = ituSceneFindWidget(&theScene, "testInBp2Sprite");
        assert(testInputSprite[4]);  
        testInputSprite[5] = ituSceneFindWidget(&theScene, "testInAccSprite");
        assert(testInputSprite[5]);

       // testLedContainer
        testInputContainer[0] = ituSceneFindWidget(&theScene, "testInRightContainer");
        assert(testInputContainer[0]);
        testInputContainer[1] = ituSceneFindWidget(&theScene, "testInLeftContainer");
        assert(testInputContainer[1]);
        testInputContainer[2] = ituSceneFindWidget(&theScene, "testInLightContainer");
        assert(testInputContainer[2]);
        testInputContainer[3] = ituSceneFindWidget(&theScene, "testInBp1Container");
        assert(testInputContainer[3] );
        testInputContainer[4]  = ituSceneFindWidget(&theScene, "testInBp2Container");
        assert(testInputContainer[4] );
        testInputContainer[5]  = ituSceneFindWidget(&theScene, "testInAccContainer");
        assert(testInputContainer[5]);

        for (i = 0; i < INPUT_MAX; i++)
        {
            Layer_Sprite_Init(testInputSprite[i], 0);
            ituWidgetSetVisible(testInputContainer[i], false);
        }
        
    }
}

void Layer_Led_Init(void)
{
    // static ITUSprite* testLedSysSprite = NULL;
    // static ITUSprite* testLedRightSprite = NULL;
    // static ITUSprite* testLedHotSprite = NULL;
    // static ITUSprite* testLedWarnSprite = NULL;
    // static ITUSprite* testLedFaultSprite = NULL;
    // static ITUSprite* testLedParkSprite = NULL;
    // static ITUSprite* testLedChgSprite = NULL;
    // static ITUSprite* testLedFarSprite = NULL;
    // static ITUSprite* testLedLowSprite = NULL;
    // static ITUSprite* testLedLeftSprite = NULL;
    // static ITUSprite* testLedAbsSprite = NULL;

    // static ITUContainer *testLedSysContainer = NULL;
    // static ITUContainer *testLedRightContainer = NULL;
    // static ITUContainer *testLedHotContainer = NULL;
    // static ITUContainer *testLedWarnContainer = NULL;
    // static ITUContainer *testLedFaultContainer = NULL;
    // static ITUContainer *testLedParkContainer = NULL;
    // static ITUContainer *testLedChgContainer = NULL;
    // static ITUContainer *testLedFarContainer = NULL;
    // static ITUContainer *testLedLowContainer = NULL;
    // static ITUContainer *testLedLeftContainer = NULL;
    // static ITUContainer *testLedAbsContainer = NULL;
    int i = 0;
    if(!testLedText1)
    {
        testLedText1 = ituSceneFindWidget(&theScene, "testLedText1");
        assert(testLedText1); 

        // testLedSprite
        testLedSprite[0] = ituSceneFindWidget(&theScene, "testLedSysSprite");
        assert(testLedSprite[0]);
        testLedSprite[1] = ituSceneFindWidget(&theScene, "testLedRightSprite");
        assert(testLedSprite[1]);
        testLedSprite[2] = ituSceneFindWidget(&theScene, "testLedHotSprite");
        assert(testLedSprite[2]);
        testLedSprite[3] = ituSceneFindWidget(&theScene, "testLedWarnSprite");
        assert(testLedSprite[3]);
        testLedSprite[4] = ituSceneFindWidget(&theScene, "testLedFaultSprite");
        assert(testLedSprite[4]);
        testLedSprite[5] = ituSceneFindWidget(&theScene, "testLedParkSprite");
        assert(testLedSprite[5]);

        testLedSprite[6] = ituSceneFindWidget(&theScene, "testLedChgSprite");
        assert(testLedSprite[6]);
        testLedSprite[7] = ituSceneFindWidget(&theScene, "testLedFarSprite");
        assert(testLedSprite[7]);
        testLedSprite[8] = ituSceneFindWidget(&theScene, "testLedLowSprite");
        assert(testLedSprite[8]);
        testLedSprite[9] = ituSceneFindWidget(&theScene, "testLedLeftSprite");
        assert(testLedSprite[9]);
        testLedSprite[10] = ituSceneFindWidget(&theScene, "testLedAbsSprite");
        assert(testLedSprite[10]);

        // testLedContainer
        testLedContainer[0] = ituSceneFindWidget(&theScene, "testLedSysContainer");
        assert(testLedContainer[0]);
        testLedContainer[1]  = ituSceneFindWidget(&theScene, "testLedRightContainer");
        assert(testLedContainer[1] );
        testLedContainer[2]  = ituSceneFindWidget(&theScene, "testLedHotContainer");
        assert(testLedContainer[2] );
        testLedContainer[3]  = ituSceneFindWidget(&theScene, "testLedWarnContainer");
        assert(testLedContainer[3] );
        testLedContainer[4]  = ituSceneFindWidget(&theScene, "testLedFaultContainer");
        assert(testLedContainer[4] );
        testLedContainer[5]  = ituSceneFindWidget(&theScene, "testLedParkContainer");
        assert(testLedContainer[5] );

        testLedContainer[6]  = ituSceneFindWidget(&theScene, "testLedChgContainer");
        assert(testLedContainer[6] );
        testLedContainer[7]  = ituSceneFindWidget(&theScene, "testLedFarContainer");
        assert(testLedContainer[7] );
        testLedContainer[8]  = ituSceneFindWidget(&theScene, "testLedLowContainer");
        assert(testLedContainer[8] );
        testLedContainer[9]  = ituSceneFindWidget(&theScene, "testLedLeftContainer");
        assert(testLedContainer[9] );
        testLedContainer[10]  = ituSceneFindWidget(&theScene, "testLedAbsContainer");
        assert(testLedContainer[10] );

        for (i = 0; i < LED_MAX; i++)
        {
            Layer_Sprite_Init(testLedSprite[i], 0);
            ituWidgetSetVisible(testLedContainer[i], false);
        }
    }
}

void Layer_Pass_Init(void)
{
    if(!testPassSprite)
    {
        testPassSprite = ituSceneFindWidget(&theScene, "testPassSprite");
        assert(testPassSprite);
        testPassText = ituSceneFindWidget(&theScene, "testPassText");
        assert(testPassText);
        testPassContainer = ituSceneFindWidget(&theScene, "testPassContainer");
        assert(testPassContainer);
        ituWidgetSetVisible(testPassContainer,false);
        Layer_Sprite_Init(testPassSprite, 0);
    }
}

bool TestModeOnEnter(ITUWidget* widget, char* param)
{
    if(!testRow1Container)
    {
        Layer_Uart_Init();
        Layer_Usb_Init();
        Layer_Psensor1_Init();
        Layer_Psensor2_Init();
        Layer_Ntc_Init();
        Layer_Rs485_Init();
        Layer_Spiflash_Init();
        Layer_Bt_Init();
        Layer_Input_Init();
        Layer_Led_Init();
        Layer_Pass_Init();

        testRow1Container = ituSceneFindWidget(&theScene, "testRow1Container");
        assert(testRow1Container);
        testRow2Container = ituSceneFindWidget(&theScene, "testRow2Container");
        assert(testRow2Container);
        testRow3Container = ituSceneFindWidget(&theScene, "testRow3Container");
        assert(testRow3Container);
        testRow4Container = ituSceneFindWidget(&theScene, "testRow4Container");
        assert(testRow4Container);
        testRow5Container = ituSceneFindWidget(&theScene, "testRow5Container");
        assert(testRow5Container);
        testRow6Container = ituSceneFindWidget(&theScene, "testRow6Container");
        assert(testRow6Container);

        ituWidgetSetVisible(testRow3Container, false);
        ituWidgetSetVisible(testRow4Container, false);
        ituWidgetSetVisible(testRow5Container, false);   
        ituWidgetSetVisible(testRow6Container, false);


        testText1 = ituSceneFindWidget(&theScene, "testText1");
        assert(testText1);
        testText2 = ituSceneFindWidget(&theScene, "testText2");
        assert(testText2);
        ituTextSetString(testText2, APP_VERSION); 
        ituTextSetString(testText1, __DATE__);
        printf("APP_VERSION:"APP_VERSION ".......\t\n");
        printf("DATE:"__DATE__ ".......\t\n");
        if (!testmodeLayer)
        {
            testmodeLayer = ituSceneFindWidget(&theScene, "testmodeLayer");
            assert(testmodeLayer);
        }

        SceneSetReady(true);
    }

    testmode_status = true;
    charge_status = false;
    upgrade_status = false;	
    return true;
}

bool TestModeOnTimer(ITUWidget* widget, char* param)
{
    // testmode_status = true;
    return false;
}
