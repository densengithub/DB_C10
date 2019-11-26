
//=============================================================================
//                Constant Definition
//=============================================================================

//=============================================================================
//                Macro Definition
//=============================================================================

//=============================================================================
//                Structure Definition
//=============================================================================

//=============================================================================
//                Global Data Definition
//=============================================================================

//=============================================================================
//                Private Function Definition
//=============================================================================

//=============================================================================
//                Public Function Definition
//=============================================================================
void
ithSensorLoadConfig(
    uint32_t configLen,
    uint8_t *configData);

void
ithPixelPlusInitialize(
    void);

void
ithPixelPlusSetEffectDefault(
    void);

void
ithPixelPlusGetEffectDefault(
    uint8_t *brightness,
    uint8_t *contrast,
    uint8_t *saturation,
    uint8_t *sharpness);

void
ithPixelPlusGetContrast(
    uint8_t *value);

void
ithPixelPlusGetBrightness(
    uint8_t *value);

void
ithPixelPlusGetSaturation(
    uint8_t *value);

void
ithPixelPlusGetSharpness(
    uint8_t *value);

void
ithPixelPlusSetContrast(
    uint8_t value);

void
ithPixelPlusSetBrightness(
    uint8_t value);

void
ithPixelPlusSetSaturation(
    uint8_t value);

void
ithPixelPlusSetSharpness(
    uint8_t value);

void
ithPixelPlusMDEnable(
    uint8_t mode);

void
ithPixelPlusMDDisable(
    void);

void
ithPixelPlusSetSection(
    uint8_t section,
    uint8_t bONflag);

void
ithPixelPlusSetSensitivity(
    uint8_t value);

void
ithPixelPlusSetMirror(
    uint8_t bEnHorMirror,
    uint8_t bEnVerMirror);
