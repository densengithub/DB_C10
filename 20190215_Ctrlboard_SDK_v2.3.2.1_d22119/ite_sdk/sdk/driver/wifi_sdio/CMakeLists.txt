if (DEFINED CFG_NET_WIFI_SDIO)
	INCLUDE_DIRECTORIES(
		${PROJECT_SOURCE_DIR}/sdk/driver/wifi_sdio
        ${PROJECT_SOURCE_DIR}/sdk/driver/wifi_sdio/include
		${PROJECT_SOURCE_DIR}/sdk/driver/wifi_sdio/include/ports
        ${PROJECT_SOURCE_DIR}/sdk/driver/wifi_sdio/include/byteorder
		${PROJECT_SOURCE_DIR}/sdk/driver/usb/inc
		${PROJECT_SOURCE_DIR}/sdk/driver/wifi_sdio/hal/OUTSRC
		${PROJECT_SOURCE_DIR}/sdk/driver/wifi_sdio/hal/OUTSRC/rtl8188e
		${PROJECT_SOURCE_DIR}/sdk/driver/wifi_sdio/os_dep/linux
		${PROJECT_SOURCE_DIR}/sdk/driver/sd/mmc/include
	)
		
	ADD_LIBRARY(wifi_sdio
		#core/
        core/rtw_ap.c
		core/rtw_cmd.c
		core/rtw_debug.c
		core/rtw_ieee80211.c
		core/rtw_io.c
		core/rtw_ioctl_query.c
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
		core/rtw_wlan_util.c
		core/rtw_xmit.c  
		core/efuse/rtw_efuse.c
		
		#os_dep/linux/
		os_dep/linux/os_intfs.c
		os_dep/linux/sdio_intf.c
		os_dep/linux/ioctl_linux.c
		os_dep/linux/xmit_linux.c
		os_dep/linux/mlme_linux.c
		os_dep/linux/recv_linux.c
        os_dep/linux/sdio_ops_linux.c
		
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
		hal/hal_hci/hal_sdio.c
		
		#hal/led/
        hal/led/hal_sdio_led.c
		
		#hal/rtl8188e/
		hal/rtl8188e/Hal8188EPwrSeq.c
		hal/rtl8188e/rtl8188e_cmd.c
		hal/rtl8188e/rtl8188e_dm.c
		hal/rtl8188e/rtl8188e_hal_init.c
		hal/rtl8188e/rtl8188e_mp.c
		hal/rtl8188e/rtl8188e_phycfg.c
		hal/rtl8188e/rtl8188e_rf6052.c
		hal/rtl8188e/rtl8188e_rxdesc.c
		hal/rtl8188e/rtl8188e_sreset.c
		hal/rtl8188e/rtl8188e_xmit.c
		
		#hal/rtl8188e/sdio
		hal/rtl8188e/sdio/rtl8189es_led.c
		hal/rtl8188e/sdio/rtl8189es_recv.c
		hal/rtl8188e/sdio/rtl8189es_xmit.c
		hal/rtl8188e/sdio/sdio_halinit.c
		hal/rtl8188e/sdio/sdio_ops.c
		
		#hal/OUTSRC/
		hal/OUTSRC/HalPhyRf.c
		hal/OUTSRC/phydm.c
		hal/OUTSRC/phydm_ACS.c
		hal/OUTSRC/PhyDM_Adaptivity.c
		hal/OUTSRC/phydm_AntDiv.c
		hal/OUTSRC/phydm_CfoTracking.c
		hal/OUTSRC/phydm_debug.c
		hal/OUTSRC/phydm_DIG.c
		hal/OUTSRC/phydm_DynamicBBPowerSaving.c
		hal/OUTSRC/phydm_DynamicTxPower.c
		hal/OUTSRC/phydm_EdcaTurboCheck.c
		hal/OUTSRC/phydm_HWConfig.c
		hal/OUTSRC/phydm_interface.c
		hal/OUTSRC/phydm_PathDiv.c
		hal/OUTSRC/phydm_PowerTracking.c
		hal/OUTSRC/phydm_RaInfo.c
		
		#hal/OUTSRC/rtl8188e
		hal/OUTSRC/rtl8188e/Hal8188ERateAdaptive.c
		hal/OUTSRC/rtl8188e/HalHWImg8188E_BB.c
		hal/OUTSRC/rtl8188e/HalHWImg8188E_FW.c
		hal/OUTSRC/rtl8188e/HalHWImg8188E_MAC.c
		hal/OUTSRC/rtl8188e/HalHWImg8188E_RF.c
		hal/OUTSRC/rtl8188e/HalPhyRf_8188e.c
		hal/OUTSRC/rtl8188e/phydm_RegConfig8188E.c
		hal/OUTSRC/rtl8188e/phydm_RTL8188E.c
		)
endif()