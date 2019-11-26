if (DEFINED CFG_NET_WIFI_5G)
    INCLUDE_DIRECTORIES(
        ${PROJECT_SOURCE_DIR}/sdk/driver/wifi_5g
        ${PROJECT_SOURCE_DIR}/sdk/driver/wifi_5g/include
        ${PROJECT_SOURCE_DIR}/sdk/driver/wifi_5g/include/ports
        ${PROJECT_SOURCE_DIR}/sdk/driver/wifi_5g/include/byteorder
        ${PROJECT_SOURCE_DIR}/sdk/driver/usb/inc
        ${PROJECT_SOURCE_DIR}/sdk/driver/wifi_5g/hal/phydm/rtl8821a
        ${PROJECT_SOURCE_DIR}/sdk/driver/wifi_5g/hal/phydm
        ${PROJECT_SOURCE_DIR}/sdk/driver/wifi_5g/os_dep/linux
        ${PROJECT_SOURCE_DIR}/sdk/
    )

    ADD_LIBRARY(wifi_5g STATIC
        #core/
        core/rtw_ap.c
        core/rtw_beamforming.c
        core/rtw_cmd.c
        core/rtw_debug.c
        #core/rtw_eeprom.c
        core/rtw_ieee80211.c
        core/rtw_io.c
        core/rtw_ioctl_query.c
        #core/rtw_ioctl_rtl.c
        core/rtw_ioctl_set.c
        core/rtw_iol.c
        core/rtw_mem.c
        core/rtw_mlme.c
        core/rtw_mlme_ext.c
        core/rtw_odm.c
        core/rtw_p2p.c
        core/rtw_pwrctrl.c
        core/rtw_recv.c
        core/rtw_rf.c
        core/rtw_security.c
        core/rtw_sreset.c
        core/rtw_sta_mgt.c
        core/rtw_vht.c
        core/rtw_wlan_util.c
        core/rtw_xmit.c

        core/efuse/rtw_efuse.c

        #os_dep/linux/
        os_dep/linux/os_intfs.c
        os_dep/linux/usb_intf.c
        os_dep/linux/ioctl_linux.c
        os_dep/linux/xmit_linux.c
        os_dep/linux/mlme_linux.c
        os_dep/linux/recv_linux.c
        os_dep/linux/usb_ops_linux.c

        os_dep/osdep_service.c

        #os_dep/ports/
        os_dep/ports/ite_ndis.c
        os_dep/ports/ite_os.c
        os_dep/ports/ite_skbuf.c
        os_dep/ports/ite_timer.c
        os_dep/ports/ite_util.c
        os_dep/ports/workqueue.c

        #hal/
        hal/hal_intf.c
        hal/hal_com.c
        hal/hal_com_phycfg.c
        hal/hal_dm.c
        hal/hal_phy.c
        hal/HalPwrSeqCmd.c

        #hal/hal_hci/
        hal/hal_hci/hal_usb.c

        #hal/led/
        hal/led/hal_usb_led.c

        #hal/rtl8812a/
        hal/rtl8812a/Hal8812PwrSeq.c
        hal/rtl8812a/Hal8821APwrSeq.c
        hal/rtl8812a/rtl8812a_cmd.c
        hal/rtl8812a/rtl8812a_dm.c
        hal/rtl8812a/rtl8812a_hal_init.c
        hal/rtl8812a/rtl8812a_phycfg.c
        hal/rtl8812a/rtl8812a_rf6052.c
        hal/rtl8812a/rtl8812a_rxdesc.c
        hal/rtl8812a/rtl8812a_sreset.c
        hal/rtl8812a/rtl8812a_xmit.c

        #hal/rtl8812a/usb/
        hal/rtl8812a/usb/rtl8812au_led.c
        hal/rtl8812a/usb/rtl8812au_recv.c
        hal/rtl8812a/usb/rtl8812au_xmit.c
        hal/rtl8812a/usb/usb_halinit.c
        hal/rtl8812a/usb/usb_ops_linux.c

        #hal/phydm/
        hal/phydm/halphyrf_ce.c
        hal/phydm/phydm.c
        hal/phydm/phydm_acs.c
        hal/phydm/phydm_adaptivity.c
        hal/phydm/phydm_antdiv.c
        hal/phydm/phydm_cfotracking.c
        hal/phydm/phydm_debug.c
        hal/phydm/phydm_dig.c
        hal/phydm/phydm_dynamicbbpowersaving.c
        hal/phydm/phydm_dynamictxpower.c
        hal/phydm/phydm_edcaturbocheck.c
        hal/phydm/phydm_HWConfig.c
        hal/phydm/phydm_interface.c
        hal/phydm/phydm_pathdiv.c
        hal/phydm/phydm_powertracking_ce.c
        hal/phydm/phydm_rainfo.c

        #hal/phydm/rtl8812a/  usb3.0 type
        #hal/phydm/rtl8812a/halhwimg8812a_bb.c
        #hal/phydm/rtl8812a/halhwimg8812a_fw.c
        #hal/phydm/rtl8812a/halhwimg8812a_mac.c
        #hal/phydm/rtl8812a/halhwimg8812a_rf.c
        #hal/phydm/rtl8812a/halphyrf_8812a_ce.c
        #hal/phydm/rtl8812a/phydm_regconfig8812a.c
        #hal/phydm/rtl8812a/phydm_rtl8812a.c

        #hal/phydm/rtl8821a/  usb2.0 type
        hal/phydm/rtl8821a/halhwimg8821a_bb.c
        hal/phydm/rtl8821a/halhwimg8821a_fw.c
        hal/phydm/rtl8821a/halhwimg8821a_mac.c
        hal/phydm/rtl8821a/halhwimg8821a_rf.c
        hal/phydm/rtl8821a/halphyrf_8821a_ce.c
        hal/phydm/rtl8821a/phydm_regconfig8821a.c
        hal/phydm/rtl8821a/phydm_rtl8821a.c
    )
endif()