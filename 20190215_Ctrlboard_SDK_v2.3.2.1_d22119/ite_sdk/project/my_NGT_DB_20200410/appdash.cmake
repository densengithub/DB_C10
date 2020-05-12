# user app
if (DEFINED CFG_LCD_POWER_ENABLE)
    add_definitions(
        -DCFG_LCD_POWER_ENABLE
        -DCFG_GPIO_LCD_POWER_CTL=${CFG_GPIO_LCD_POWER_CTL} 
        -DCFG_GPIO_LCD_RESET_CTL=${CFG_GPIO_LCD_RESET_CTL}         
        )
endif()

add_definitions(
    -DCFG_NIU_PANEL_VERSION="${CFG_NIU_PANEL_VERSION}"
    )