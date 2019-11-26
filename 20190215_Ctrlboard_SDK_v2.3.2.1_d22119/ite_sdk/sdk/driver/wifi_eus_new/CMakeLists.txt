if (DEFINED CFG_NET_WIFI_8188EUS_NEW)
	INCLUDE_DIRECTORIES(
		${PROJECT_SOURCE_DIR}/sdk/driver/wifi_eus_new
        ${PROJECT_SOURCE_DIR}/sdk/driver/wifi_eus_new/include
        ${PROJECT_SOURCE_DIR}/sdk/driver/wifi_eus_new/include/ports
		${PROJECT_SOURCE_DIR}/sdk/driver/wifi_eus_new/include/cmn_info
        ${PROJECT_SOURCE_DIR}/sdk/driver/wifi_eus_new/include/byteorder
        ${PROJECT_SOURCE_DIR}/sdk/driver/usb/inc
		${PROJECT_SOURCE_DIR}/sdk/driver/wifi_eus_new/hal
		${PROJECT_SOURCE_DIR}/sdk/driver/wifi_eus_new/hal/phydm
        ${PROJECT_SOURCE_DIR}/sdk/driver/wifi_eus_new/hal/phydm/rtl8188e
		${PROJECT_SOURCE_DIR}/sdk/driver/wifi_eus_new/hal/phydm/halrf
		${PROJECT_SOURCE_DIR}/sdk/driver/wifi_eus_new/hal/phydm/halrf/rtl8188e
		${PROJECT_SOURCE_DIR}/sdk/driver/wifi_eus_new/hal/efuse/
		${PROJECT_SOURCE_DIR}/sdk/driver/wifi_eus_new/hal/efuse/rtl8188e
		${PROJECT_SOURCE_DIR}/sdk/driver/wifi_eus_new/hal/rtl8188e
        ${PROJECT_SOURCE_DIR}/sdk/driver/wifi_eus_new/os_dep/linux
		)
		
	ADD_LIBRARY(wifi_eus_new STATIC
		#core/
        core/rtw_ap.c
		core/rtw_ieee80211.c
		core/rtw_cmd.c
		core/rtw_debug.c
		core/rtw_io.c
		core/rtw_ioctl_query.c
		core/rtw_ioctl_set.c
		core/rtw_iol.c
		core/rtw_mi.c
		core/rtw_mlme.c
		core/rtw_mlme_ext.c
		core/rtw_pwrctrl.c
		core/rtw_recv.c
		core/rtw_rf.c
		core/rtw_rm.c
		core/rtw_security.c
		core/rtw_sta_mgt.c
		core/rtw_wlan_util.c
		core/rtw_xmit.c
		core/rtw_sreset.c
		core/rtw_security.c
		core/rtw_odm.c
		core/rtw_p2p.c
		
		core/efuse/rtw_efuse.c
		
		#os_dep/
		os_dep/osdep_service.c
		
		#os_dep/linux/
		os_dep/linux/os_intfs.c
		os_dep/linux/usb_intf.c
		os_dep/linux/ioctl_linux.c
		os_dep/linux/xmit_linux.c
		os_dep/linux/mlme_linux.c
		os_dep/linux/recv_linux.c
        os_dep/linux/usb_ops_linux.c

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
		hal/hal_phy.c
		hal/hal_intf.c
		hal/HalPwrSeqCmd.c
		
		#hal/hal_hci/
		hal/hal_hci/hal_usb.c
		
		#hal/led/
		hal/led/hal_led.c
        hal/led/hal_usb_led.c
		
		#hal/phydm/
        hal/phydm/phydm.c
        #hal/phydm/phydm_acs.c
        hal/phydm/phydm_adaptivity.c
        hal/phydm/phydm_antdiv.c
		hal/phydm/phydm_api.c
		hal/phydm/phydm_auto_dbg.c
		#hal/phydm/phydm_beamforming.c
		hal/phydm/phydm_cck_pd.c
		hal/phydm/phydm_ccx.c
        hal/phydm/phydm_cfotracking.c
        hal/phydm/phydm_debug.c
		hal/phydm/phydm_dfs.c
        hal/phydm/phydm_dig.c
		hal/phydm/phydm_dynamic_rx_path.c
        hal/phydm/phydm_dynamictxpower.c
        hal/phydm/phydm_hwconfig.c
        hal/phydm/phydm_interface.c
		hal/phydm/phydm_math_lib.c
        hal/phydm/phydm_pathdiv.c
		hal/phydm/phydm_phystatus.c
		hal/phydm/phydm_primary_cca.c
		hal/phydm/phydm_psd.c
        hal/phydm/phydm_rainfo.c
		hal/phydm/phydm_rssi_monitor.C
		hal/phydm/phydm_soml.c

		#hal/phydm/rtl8188e/
		hal/phydm/rtl8188e/hal8188erateadaptive.c
		hal/phydm/rtl8188e/halhwimg8188e_bb.c
		hal/phydm/rtl8188e/halhwimg8188e_mac.c
		hal/phydm/rtl8188e/halhwimg8188e_rf.c
		hal/phydm/rtl8188e/phydm_regconfig8188e.c
		hal/phydm/rtl8188e/phydm_rtl8188e.c

		#hal/rtl8188e/
		hal/rtl8188e/hal8188e_s_fw.c
		hal/rtl8188e/hal8188e_t_fw.c
		hal/rtl8188e/Hal8188EPwrSeq.c
		hal/rtl8188e/rtl8188e_cmd.c
		hal/rtl8188e/rtl8188e_dm.c
		hal/rtl8188e/rtl8188e_hal_init.c
		hal/rtl8188e/rtl8188e_phycfg.c
		hal/rtl8188e/rtl8188e_rf6052.c
		hal/rtl8188e/rtl8188e_rxdesc.c
		hal/rtl8188e/rtl8188e_sreset.c
		hal/rtl8188e/rtl8188e_xmit.c
		
		#hal/phydm/halrf/
		hal/phydm/halrf/halrf_kfree.c
		hal/phydm/halrf/halrf_powertracking.c
		hal/phydm/halrf/halrf_powertracking_ce.c
		hal/phydm/halrf/halrf.c
		hal/phydm/halrf/halphyrf_ce.c
		hal/phydm/halrf/halrf_kfree.c
		
		
		#hal/phydm/halrf/rtl8188e
		hal/phydm/halrf/rtl8188e/halrf_8188e_ce.c
		
		#hal/efuse/rtl8188e
		hal/efuse/rtl8188e/halEfuseMask8188E_USB.c
		
		#hal/rtl8188e/usb/
		hal/rtl8188e/usb/rtl8188eu_led.c
		hal/rtl8188e/usb/rtl8188eu_recv.c
		hal/rtl8188e/usb/rtl8188eu_xmit.c
		hal/rtl8188e/usb/usb_halinit.c
		hal/rtl8188e/usb/usb_ops_linux.c
		)
endif()