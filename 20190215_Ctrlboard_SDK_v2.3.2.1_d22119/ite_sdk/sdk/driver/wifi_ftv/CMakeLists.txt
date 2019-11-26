if (DEFINED CFG_NET_WIFI_ftv)
    INCLUDE_DIRECTORIES(
        ${PROJECT_SOURCE_DIR}/sdk/driver/wifi_ftv
        ${PROJECT_SOURCE_DIR}/sdk/driver/wifi_ftv/include
        ${PROJECT_SOURCE_DIR}/sdk/driver/wifi_ftv/include/ports
        ${PROJECT_SOURCE_DIR}/sdk/driver/wifi_ftv/include/byteorder
        ${PROJECT_SOURCE_DIR}/sdk/driver/usb/inc
        ${PROJECT_SOURCE_DIR}/sdk/driver/wifi_ftv/hal/phydm/rtl8188f
        ${PROJECT_SOURCE_DIR}/sdk/driver/wifi_ftv/hal/phydm
        ${PROJECT_SOURCE_DIR}/sdk/driver/wifi_ftv/os_dep/linux
    )

    ADD_LIBRARY(wifi_ftv STATIC
        #core/
        core/rtw_ap.c
        #core/rtw_beamforming.c
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
		core/rtw_mi.c
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

		#core/efuse/
        core/efuse/rtw_efuse.c

        #os_dep/linux/
        os_dep/linux/ioctl_linux.c
        os_dep/linux/mlme_linux.c
        os_dep/linux/os_intfs.c
        os_dep/linux/recv_linux.c
		os_dep/linux/usb_intf.c
        os_dep/linux/usb_ops_linux.c
		os_dep/linux/xmit_linux.c

		#os_dep/
        os_dep/osdep_service.c

        #os_dep/ports/
        os_dep/ports/ite_ndis.c
        os_dep/ports/ite_os.c
        os_dep/ports/ite_skbuf.c
        os_dep/ports/ite_timer.c
        os_dep/ports/ite_util.c
        os_dep/ports/workqueue.c

        #hal/
        
        hal/hal_com.c
        hal/hal_com_phycfg.c
        hal/hal_dm.c
		hal/hal_halmac.c
		hal/hal_intf.c
		#hal/hal_mcc.c
        hal/hal_phy.c
        hal/HalPwrSeqCmd.c

        #hal/hal_hci/
        hal/hal_hci/hal_usb.c

        #hal/led/
        hal/led/hal_usb_led.c

        #hal/rtl8188f/
        hal/rtl8188f/Hal8188FPwrSeq.c
        hal/rtl8188f/rtl8188f_cmd.c
        hal/rtl8188f/rtl8188f_dm.c
        hal/rtl8188f/rtl8188f_hal_init.c
        hal/rtl8188f/rtl8188f_phycfg.c
        hal/rtl8188f/rtl8188f_rf6052.c
        hal/rtl8188f/rtl8188f_rxdesc.c
        hal/rtl8188f/rtl8188f_sreset.c

        #hal/rtl8188f/usb/
        hal/rtl8188f/usb/rtl8188fu_led.c
        hal/rtl8188f/usb/rtl8188fu_recv.c
        hal/rtl8188f/usb/rtl8188fu_xmit.c
        hal/rtl8188f/usb/usb_halinit.c
        hal/rtl8188f/usb/usb_ops.c

        #hal/phydm/
        hal/phydm/halphyrf_ce.c
        hal/phydm/phydm.c
        hal/phydm/phydm_acs.c
        hal/phydm/phydm_adaptivity.c
        hal/phydm/phydm_antdiv.c
		#hal/phydm/phydm_beamforming.c
		hal/phydm/phydm_ccx.c
        hal/phydm/phydm_cfotracking.c
        hal/phydm/phydm_debug.c
		hal/phydm/phydm_dfs.c
        hal/phydm/phydm_dig.c
		hal/phydm/phydm_dynamic_rx_path.c
        hal/phydm/phydm_dynamicbbpowersaving.c
        hal/phydm/phydm_dynamictxpower.c
        hal/phydm/phydm_edcaturbocheck.c
        hal/phydm/phydm_hwconfig.c
        hal/phydm/phydm_interface.c
        hal/phydm/phydm_pathdiv.c
        hal/phydm/phydm_powertracking_ce.c
		hal/phydm/phydm_psd.c
        hal/phydm/phydm_rainfo.c

        #hal/phydm/rtl8188f/  usb2.0 type
        hal/phydm/rtl8188f/halhwimg8188f_bb.c
        hal/phydm/rtl8188f/halhwimg8188f_fw.c
        hal/phydm/rtl8188f/halhwimg8188f_mac.c
        hal/phydm/rtl8188f/halhwimg8188f_rf.c
        hal/phydm/rtl8188f/halphyrf_8188f.c
        hal/phydm/rtl8188f/phydm_regconfig8188f.c
        hal/phydm/rtl8188f/phydm_rtl8188f.c
    )
endif()