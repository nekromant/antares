
// The contents of this file are believed to come from a linux PRISM2 driver,
// with some changes made by Pascal Stang.

/*--- Record ID Constants --------------------------*/
/*--------------------------------------------------------------------
Configuration RIDs: Network Parameters, Static Configuration Entities
--------------------------------------------------------------------*/
#define	PRISM2_RID_CNFPORTTYPE				((UINT16)0xFC00)
#define	PRISM2_RID_CNFOWNMACADDR			((UINT16)0xFC01)
#define	PRISM2_RID_CNFDESIREDSSID			((UINT16)0xFC02)
#define	PRISM2_RID_CNFOWNCHANNEL			((UINT16)0xFC03)
#define	PRISM2_RID_CNFOWNSSID				((UINT16)0xFC04)
#define	PRISM2_RID_CNFOWNATIMWIN			((UINT16)0xFC05)
#define	PRISM2_RID_CNFSYSSCALE				((UINT16)0xFC06)
#define	PRISM2_RID_CNFMAXDATALEN			((UINT16)0xFC07)
#define	PRISM2_RID_CNFWDSADDR				((UINT16)0xFC08)
#define	PRISM2_RID_CNFPMENABLED				((UINT16)0xFC09)
#define	PRISM2_RID_CNFPMEPS					((UINT16)0xFC0A)
#define	PRISM2_RID_CNFMULTICASTRX			((UINT16)0xFC0B)
#define	PRISM2_RID_CNFMAXSLEEPDUR			((UINT16)0xFC0C)
#define	PRISM2_RID_CNFPMHOLDDUR				((UINT16)0xFC0D)
#define	PRISM2_RID_CNFOWNNAME				((UINT16)0xFC0E)
#define	PRISM2_RID_CNFOWNDTIMPER			((UINT16)0xFC10)
#define	PRISM2_RID_CNFWDSADDR1				((UINT16)0xFC11)
#define	PRISM2_RID_CNFWDSADDR2				((UINT16)0xFC12)
#define	PRISM2_RID_CNFWDSADDR3				((UINT16)0xFC13)
#define	PRISM2_RID_CNFWDSADDR4				((UINT16)0xFC14)
#define	PRISM2_RID_CNFWDSADDR5				((UINT16)0xFC15)
#define	PRISM2_RID_CNFWDSADDR6				((UINT16)0xFC16)
#define	PRISM2_RID_CNFMCASTPMBUFF			((UINT16)0xFC17)

/*--------------------------------------------------------------------
Configuration RID lengths: Network Params, Static Config Entities
  This is the length of JUST the DATA part of the RID (does not 
  include the len or code fields)
--------------------------------------------------------------------*/
/* TODO: fill in the rest of these */
#define	PRISM2_RID_CNFPORTTYPE_LEN			((UINT16)2)
#define	PRISM2_RID_CNFOWNMACADDR_LEN		((UINT16)6)
#define	PRISM2_RID_CNFDESIREDSSID_LEN		((UINT16)34)
#define	PRISM2_RID_CNFOWNCHANNEL_LEN		((UINT16)2)
#define	PRISM2_RID_CNFOWNSSID_LEN			((UINT16)34)
#define	PRISM2_RID_CNFOWNATIMWIN_LEN		((UINT16)2)
#define	PRISM2_RID_CNFSYSSCALE_LEN			((UINT16)0)
#define	PRISM2_RID_CNFMAXDATALEN_LEN		((UINT16)0)
#define	PRISM2_RID_CNFWDSADDR_LEN			((UINT16)6)
#define	PRISM2_RID_CNFPMENABLED_LEN			((UINT16)0)
#define	PRISM2_RID_CNFPMEPS_LEN				((UINT16)0)
#define	PRISM2_RID_CNFMULTICASTRX_LEN		((UINT16)0)
#define	PRISM2_RID_CNFMAXSLEEPDUR_LEN		((UINT16)0)
#define	PRISM2_RID_CNFPMHOLDDUR_LEN			((UINT16)0)
#define	PRISM2_RID_CNFOWNNAME_LEN			((UINT16)34)
#define	PRISM2_RID_CNFOWNDTIMPER_LEN		((UINT16)0)
#define	PRISM2_RID_CNFWDSADDR1_LEN			((UINT16)6)
#define	PRISM2_RID_CNFWDSADDR2_LEN			((UINT16)6)
#define	PRISM2_RID_CNFWDSADDR3_LEN			((UINT16)6)
#define	PRISM2_RID_CNFWDSADDR4_LEN			((UINT16)6)
#define	PRISM2_RID_CNFWDSADDR5_LEN			((UINT16)6)
#define	PRISM2_RID_CNFWDSADDR6_LEN			((UINT16)6)
#define	PRISM2_RID_CNFMCASTPMBUFF_LEN		((UINT16)0)
#define	PRISM2_RID_CNFAUTHENTICATION_LEN	((UINT16)sizeof(UINT16))
#define	PRISM2_RID_CNFMAXSLEEPDUR_LEN		((UINT16)0)

/*--------------------------------------------------------------------
Configuration RIDs: Network Parameters, Dynamic Configuration Entities
--------------------------------------------------------------------*/
#define	PRISM2_RID_GROUPADDR				((UINT16)0xFC80)
#define	PRISM2_RID_CREATEIBSS				((UINT16)0xFC81)
#define	PRISM2_RID_FRAGTHRESH				((UINT16)0xFC82)
#define	PRISM2_RID_RTSTHRESH				((UINT16)0xFC83)
#define	PRISM2_RID_TXRATECNTL				((UINT16)0xFC84)
#define	PRISM2_RID_PROMISCMODE				((UINT16)0xFC85)
#define	PRISM2_RID_FRAGTHRESH0				((UINT16)0xFC90)
#define	PRISM2_RID_FRAGTHRESH1				((UINT16)0xFC91)
#define	PRISM2_RID_FRAGTHRESH2				((UINT16)0xFC92)
#define	PRISM2_RID_FRAGTHRESH3				((UINT16)0xFC93)
#define	PRISM2_RID_FRAGTHRESH4				((UINT16)0xFC94)
#define	PRISM2_RID_FRAGTHRESH5				((UINT16)0xFC95)
#define	PRISM2_RID_FRAGTHRESH6				((UINT16)0xFC96)
#define	PRISM2_RID_RTSTHRESH0				((UINT16)0xFC97)
#define	PRISM2_RID_RTSTHRESH1				((UINT16)0xFC98)
#define	PRISM2_RID_RTSTHRESH2				((UINT16)0xFC99)
#define	PRISM2_RID_RTSTHRESH3				((UINT16)0xFC9A)
#define	PRISM2_RID_RTSTHRESH4				((UINT16)0xFC9B)
#define	PRISM2_RID_RTSTHRESH5				((UINT16)0xFC9C)
#define	PRISM2_RID_RTSTHRESH6				((UINT16)0xFC9D)
#define	PRISM2_RID_TXRATECNTL0				((UINT16)0xFC9E)
#define	PRISM2_RID_TXRATECNTL1				((UINT16)0xFC9F)
#define	PRISM2_RID_TXRATECNTL2				((UINT16)0xFCA0)
#define	PRISM2_RID_TXRATECNTL3				((UINT16)0xFCA1)
#define	PRISM2_RID_TXRATECNTL4				((UINT16)0xFCA2)
#define	PRISM2_RID_TXRATECNTL5				((UINT16)0xFCA3)
#define	PRISM2_RID_TXRATECNTL6				((UINT16)0xFCA4)

/*--------------------------------------------------------------------
Configuration RID Lengths: Network Param, Dynamic Config Entities
  This is the length of JUST the DATA part of the RID (does not 
  include the len or code fields)
--------------------------------------------------------------------*/
/* TODO: fill in the rest of these */
#define	PRISM2_RID_GROUPADDR_LEN			((UINT16)16 * WLAN_ADDR_LEN)
#define	PRISM2_RID_CREATEIBSS_LEN			((UINT16)0)
#define	PRISM2_RID_FRAGTHRESH_LEN			((UINT16)0)
#define	PRISM2_RID_RTSTHRESH_LEN			((UINT16)0)
#define	PRISM2_RID_TXRATECNTL_LEN			((UINT16)4)
#define	PRISM2_RID_PROMISCMODE_LEN			((UINT16)2)
#define	PRISM2_RID_FRAGTHRESH0_LEN			((UINT16)0)
#define	PRISM2_RID_FRAGTHRESH1_LEN			((UINT16)0)
#define	PRISM2_RID_FRAGTHRESH2_LEN			((UINT16)0)
#define	PRISM2_RID_FRAGTHRESH3_LEN			((UINT16)0)
#define	PRISM2_RID_FRAGTHRESH4_LEN			((UINT16)0)
#define	PRISM2_RID_FRAGTHRESH5_LEN			((UINT16)0)
#define	PRISM2_RID_FRAGTHRESH6_LEN			((UINT16)0)
#define	PRISM2_RID_RTSTHRESH0_LEN			((UINT16)0)
#define	PRISM2_RID_RTSTHRESH1_LEN			((UINT16)0)
#define	PRISM2_RID_RTSTHRESH2_LEN			((UINT16)0)
#define	PRISM2_RID_RTSTHRESH3_LEN			((UINT16)0)
#define	PRISM2_RID_RTSTHRESH4_LEN			((UINT16)0)
#define	PRISM2_RID_RTSTHRESH5_LEN			((UINT16)0)
#define	PRISM2_RID_RTSTHRESH6_LEN			((UINT16)0)
#define	PRISM2_RID_TXRATECNTL0_LEN			((UINT16)0)
#define	PRISM2_RID_TXRATECNTL1_LEN			((UINT16)0)
#define	PRISM2_RID_TXRATECNTL2_LEN			((UINT16)0)
#define	PRISM2_RID_TXRATECNTL3_LEN			((UINT16)0)
#define	PRISM2_RID_TXRATECNTL4_LEN			((UINT16)0)
#define	PRISM2_RID_TXRATECNTL5_LEN			((UINT16)0)
#define	PRISM2_RID_TXRATECNTL6_LEN			((UINT16)0)

/*--------------------------------------------------------------------
Configuration RIDs: Behavior Parameters
--------------------------------------------------------------------*/
#define		PRISM2_RID_ITICKTIME			((UINT16)0xFCE0)

/*--------------------------------------------------------------------
Configuration RID Lengths: Behavior Parameters
  This is the length of JUST the DATA part of the RID (does not 
  include the len or code fields)
--------------------------------------------------------------------*/
#define		PRISM2_RID_ITICKTIME_LEN		((UINT16)2)

/*----------------------------------------------------------------------
Information RIDs: NIC Information
--------------------------------------------------------------------*/
#define	PRISM2_RID_MAXLOADTIME				((UINT16)0xFD00)
#define	PRISM2_RID_DOWNLOADBUFFER			((UINT16)0xFD01)
#define	PRISM2_RID_PRIIDENTITY				((UINT16)0xFD02)
#define	PRISM2_RID_PRISUPRANGE				((UINT16)0xFD03)
#define	PRISM2_RID_PRI_CFIACTRANGES			((UINT16)0xFD04)
#define	PRISM2_RID_NICSERIALNUMBER			((UINT16)0xFD0A)
#define	PRISM2_RID_NICIDENTITY				((UINT16)0xFD0B)
#define	PRISM2_RID_MFISUPRANGE				((UINT16)0xFD0C)
#define	PRISM2_RID_CFISUPRANGE				((UINT16)0xFD0D)
#define	PRISM2_RID_CHANNELLIST				((UINT16)0xFD10)
#define	PRISM2_RID_REGULATORYDOMAINS		((UINT16)0xFD11)
#define	PRISM2_RID_TEMPTYPE					((UINT16)0xFD12)
#define	PRISM2_RID_CIS						((UINT16)0xFD13)
#define	PRISM2_RID_STAIDENTITY				((UINT16)0xFD20)
#define	PRISM2_RID_STASUPRANGE				((UINT16)0xFD21)
#define	PRISM2_RID_STA_MFIACTRANGES			((UINT16)0xFD22)
#define	PRISM2_RID_STA_CFIACTRANGES			((UINT16)0xFD23)
#define	PRISM2_RID_BUILDSEQ					((UINT16)0xFFFE)
#define	PRISM2_RID_FWID						((UINT16)0xFFFF)

/*----------------------------------------------------------------------
Information RID Lengths: NIC Information
  This is the length of JUST the DATA part of the RID (does not 
  include the len or code fields)
--------------------------------------------------------------------*/
#define	PRISM2_RID_MAXLOADTIME_LEN			((UINT16)0)
#define	PRISM2_RID_DOWNLOADBUFFER_LEN		((UINT16)sizeof(PRISM2_downloadbuffer_t))
#define	PRISM2_RID_PRIIDENTITY_LEN			((UINT16)8)
#define	PRISM2_RID_PRISUPRANGE_LEN			((UINT16)10)
#define	PRISM2_RID_CFIACTRANGES_LEN			((UINT16)10)
#define	PRISM2_RID_NICSERIALNUMBER_LEN		((UINT16)12)
#define	PRISM2_RID_NICIDENTITY_LEN			((UINT16)8)
#define	PRISM2_RID_MFISUPRANGE_LEN			((UINT16)10)
#define	PRISM2_RID_CFISUPRANGE_LEN			((UINT16)10)
#define	PRISM2_RID_CHANNELLIST_LEN			((UINT16)0)
#define	PRISM2_RID_REGULATORYDOMAINS_LEN	((UINT16)12)
#define	PRISM2_RID_TEMPTYPE_LEN				((UINT16)0)
#define	PRISM2_RID_CIS_LEN					((UINT16)480)
#define	PRISM2_RID_STAIDENTITY_LEN			((UINT16)8)
#define	PRISM2_RID_STASUPRANGE_LEN			((UINT16)10)
#define	PRISM2_RID_MFIACTRANGES_LEN			((UINT16)10)
#define	PRISM2_RID_CFIACTRANGES2_LEN		((UINT16)10)
#define	PRISM2_RID_BUILDSEQ_LEN				((UINT16)sizeof(PRISM2_BuildSeq_t))
#define	PRISM2_RID_FWID_LEN					((UINT16)sizeof(PRISM2_FWID_t))

/*--------------------------------------------------------------------
Information RIDs:  MAC Information
--------------------------------------------------------------------*/
#define	PRISM2_RID_PORTSTATUS				((UINT16)0xFD40)
#define	PRISM2_RID_CURRENTSSID				((UINT16)0xFD41)
#define	PRISM2_RID_CURRENTBSSID				((UINT16)0xFD42)
#define	PRISM2_RID_COMMSQUALITY				((UINT16)0xFD43)
#define	PRISM2_RID_CURRENTTXRATE			((UINT16)0xFD44)
#define	PRISM2_RID_CURRENTBCNINT			((UINT16)0xFD45)
#define	PRISM2_RID_CURRENTSCALETHRESH		((UINT16)0xFD46)
#define	PRISM2_RID_PROTOCOLRSPTIME			((UINT16)0xFD47)
#define	PRISM2_RID_SHORTRETRYLIMIT			((UINT16)0xFD48)
#define	PRISM2_RID_LONGRETRYLIMIT			((UINT16)0xFD49)
#define	PRISM2_RID_MAXTXLIFETIME			((UINT16)0xFD4A)
#define	PRISM2_RID_MAXRXLIFETIME			((UINT16)0xFD4B)
#define	PRISM2_RID_CFPOLLABLE				((UINT16)0xFD4C)
#define	PRISM2_RID_AUTHALGORITHMS			((UINT16)0xFD4D)
#define	PRISM2_RID_PRIVACYOPTIMP			((UINT16)0xFD4F)
#define	PRISM2_RID_DBMCOMMSQUALITY			((UINT16)0xFD51)
#define	PRISM2_RID_CURRENTTXRATE1			((UINT16)0xFD80)
#define	PRISM2_RID_CURRENTTXRATE2			((UINT16)0xFD81)
#define	PRISM2_RID_CURRENTTXRATE3			((UINT16)0xFD82)
#define	PRISM2_RID_CURRENTTXRATE4			((UINT16)0xFD83)
#define	PRISM2_RID_CURRENTTXRATE5			((UINT16)0xFD84)
#define	PRISM2_RID_CURRENTTXRATE6			((UINT16)0xFD85)
#define	PRISM2_RID_OWNMACADDRESS			((UINT16)0xFD86)
// #define	PRISM2_RID_PCFINFO				((UINT16)0xFD87)
#define	PRISM2_RID_SCANRESULTS       		((UINT16)0xFD88) // NEW
#define	PRISM2_RID_HOSTSCANRESULTS   		((UINT16)0xFD89) // NEW
#define	PRISM2_RID_AUTHENTICATIONUSED		((UINT16)0xFD8A) // NEW

/*--------------------------------------------------------------------
Information RID Lengths:  MAC Information
  This is the length of JUST the DATA part of the RID (does not 
  include the len or code fields)
--------------------------------------------------------------------*/
#define	PRISM2_RID_PORTSTATUS_LEN			((UINT16)0)
#define	PRISM2_RID_CURRENTSSID_LEN			((UINT16)34)
#define	PRISM2_RID_CURRENTBSSID_LEN			((UINT16)WLAN_BSSID_LEN)
#define	PRISM2_RID_COMMSQUALITY_LEN			((UINT16)sizeof(PRISM2_commsquality_t))
#define	PRISM2_RID_DBMCOMMSQUALITY_LEN		((UINT16)sizeof(PRISM2_dbmcommsquality_t))
#define	PRISM2_RID_CURRENTTXRATE_LEN		((UINT16)0)
#define	PRISM2_RID_CURRENTBCNINT_LEN		((UINT16)0)
#define	PRISM2_RID_STACURSCALETHRESH_LEN	((UINT16)12)
#define	PRISM2_RID_APCURSCALETHRESH_LEN		((UINT16)6)
#define	PRISM2_RID_PROTOCOLRSPTIME_LEN		((UINT16)0)
#define	PRISM2_RID_SHORTRETRYLIMIT_LEN		((UINT16)0)
#define	PRISM2_RID_LONGRETRYLIMIT_LEN		((UINT16)0)
#define	PRISM2_RID_MAXTXLIFETIME_LEN		((UINT16)0)
#define	PRISM2_RID_MAXRXLIFETIME_LEN		((UINT16)0)
#define	PRISM2_RID_CFPOLLABLE_LEN			((UINT16)0)
#define	PRISM2_RID_AUTHALGORITHMS_LEN		((UINT16)4)
#define	PRISM2_RID_PRIVACYOPTIMP_LEN		((UINT16)0)
#define	PRISM2_RID_CURRENTTXRATE1_LEN		((UINT16)0)
#define	PRISM2_RID_CURRENTTXRATE2_LEN		((UINT16)0)
#define	PRISM2_RID_CURRENTTXRATE3_LEN		((UINT16)0)
#define	PRISM2_RID_CURRENTTXRATE4_LEN		((UINT16)0)
#define	PRISM2_RID_CURRENTTXRATE5_LEN		((UINT16)0)
#define	PRISM2_RID_CURRENTTXRATE6_LEN		((UINT16)0)
#define	PRISM2_RID_OWNMACADDRESS_LEN		((UINT16)6)
#define	PRISM2_RID_PCFINFO_LEN				((UINT16)6)
#define	PRISM2_RID_CNFAPPCFINFO_LEN			((UINT16)sizeof(PRISM2_PCFInfo_data_t))
#define	PRISM2_RID_SCANREQUEST_LEN			((UINT16)sizeof(PRISM2_ScanRequest_data_t))
#define	PRISM2_RID_JOINREQUEST_LEN			((UINT16)sizeof(PRISM2_JoinRequest_data_t))
#define	PRISM2_RID_AUTHENTICATESTA_LEN		((UINT16)sizeof(PRISM2_authenticateStation_data_t))
#define	PRISM2_RID_CHANNELINFOREQUEST_LEN	((UINT16)sizeof(PRISM2_ChannelInfoRequest_data_t))
/*--------------------------------------------------------------------
Information RIDs:  Modem Information
--------------------------------------------------------------------*/
#define	PRISM2_RID_PHYTYPE					((UINT16)0xFDC0)
#define	PRISM2_RID_CURRENTCHANNEL			((UINT16)0xFDC1)
#define	PRISM2_RID_CURRENTPOWERSTATE		((UINT16)0xFDC2)
#define	PRISM2_RID_CCAMODE					((UINT16)0xFDC3)
#define	PRISM2_RID_SUPPORTEDDATARATES		((UINT16)0xFDC6)
#define	PRISM2_RID_LFOSTATUS				((UINT16)0xFDC7) // 1.7.1

/*--------------------------------------------------------------------
Information RID Lengths:  Modem Information 
  This is the length of JUST the DATA part of the RID (does not 
  include the len or code fields)
--------------------------------------------------------------------*/
#define	PRISM2_RID_PHYTYPE_LEN				((UINT16)0)
#define	PRISM2_RID_CURRENTCHANNEL_LEN		((UINT16)0)
#define	PRISM2_RID_CURRENTPOWERSTATE_LEN	((UINT16)0)
#define	PRISM2_RID_CCAMODE_LEN				((UINT16)0)
#define	PRISM2_RID_SUPPORTEDDATARATES_LEN	((UINT16)10)

/*--------------------------------------------------------------------
API ENHANCEMENTS (NOT ALREADY IMPLEMENTED)
--------------------------------------------------------------------*/
#define	PRISM2_RID_CNFWEPDEFAULTKEYID		((UINT16)0xFC23)
#define	PRISM2_RID_CNFWEPDEFAULTKEY0		((UINT16)0xFC24)
#define	PRISM2_RID_CNFWEPDEFAULTKEY1		((UINT16)0xFC25)
#define	PRISM2_RID_CNFWEPDEFAULTKEY2		((UINT16)0xFC26)
#define	PRISM2_RID_CNFWEPDEFAULTKEY3		((UINT16)0xFC27)
#define	PRISM2_RID_CNFWEPFLAGS				((UINT16)0xFC28)
#define	PRISM2_RID_CNFWEPKEYMAPTABLE		((UINT16)0xFC29)
#define	PRISM2_RID_CNFAUTHENTICATION		((UINT16)0xFC2A)
#define	PRISM2_RID_CNFMAXASSOCSTATIONS		((UINT16)0xFC2B)
#define	PRISM2_RID_CNFTXCONTROL				((UINT16)0xFC2C)
#define	PRISM2_RID_CNFROAMINGMODE			((UINT16)0xFC2D)
#define	PRISM2_RID_CNFHOSTAUTH				((UINT16)0xFC2E)
#define	PRISM2_RID_CNFRCVCRCERROR			((UINT16)0xFC30)
// #define PRISM2_RID_CNFMMLIFE				((UINT16)0xFC31)
#define	PRISM2_RID_CNFALTRETRYCNT			((UINT16)0xFC32)
#define	PRISM2_RID_CNFAPBCNINT				((UINT16)0xFC33)
#define	PRISM2_RID_CNFAPPCFINFO				((UINT16)0xFC34)
#define	PRISM2_RID_CNFSTAPCFINFO			((UINT16)0xFC35)
#define	PRISM2_RID_CNFPRIORITYQUSAGE		((UINT16)0xFC37)
#define	PRISM2_RID_CNFTIMCTRL				((UINT16)0xFC40)
#define	PRISM2_RID_CNFTHIRTY2TALLY			((UINT16)0xFC42)
#define	PRISM2_RID_CNFENHSECURITY			((UINT16)0xFC43)
#define	PRISM2_RID_CNFDBMADJUST  			((UINT16)0xFC46) // NEW
#define	PRISM2_RID_SSNGENERICELEMENT		((UINT16)0xFC48) // 1.7.0
#define	PRISM2_RID_CNFSHORTPREAMBLE			((UINT16)0xFCB0)
#define	PRISM2_RID_CNFEXCLONGPREAMBLE		((UINT16)0xFCB1)
#define	PRISM2_RID_CNFAUTHRSPTIMEOUT		((UINT16)0xFCB2)
#define	PRISM2_RID_CNFBASICRATES			((UINT16)0xFCB3)
#define	PRISM2_RID_CNFSUPPRATES				((UINT16)0xFCB4) 
#define	PRISM2_RID_CNFFALLBACKCTRL			((UINT16)0xFCB5) // NEW 
#define	PRISM2_RID_WEPKEYDISABLE  			((UINT16)0xFCB6) // NEW
#define	PRISM2_RID_WEPKEYMAPINDEX 			((UINT16)0xFCB7) // NEW
#define	PRISM2_RID_BROADCASTKEYID 			((UINT16)0xFCB8) // NEW 
#define	PRISM2_RID_ENTSECFLAGEYID 			((UINT16)0xFCB9) // NEW
#define	PRISM2_RID_CNFPASSIVESCANCTRL		((UINT16)0xFCBA) // NEW STA
#define	PRISM2_RID_SSNHANDLINGMODE			((UINT16)0xFCBB) // 1.7.0
#define	PRISM2_RID_MDCCONTROL        		((UINT16)0xFCBC) // 1.7.0/1.4.0
#define	PRISM2_RID_MDCCOUNTRY        		((UINT16)0xFCBD) // 1.7.0/1.4.0
#define	PRISM2_RID_TXPOWERMAX        		((UINT16)0xFCBE) // 1.7.0/1.4.0
#define	PRISM2_RID_CNFLFOENBLED      		((UINT16)0xFCBF) // 1.6.3
#define PRISM2_RID_CAPINFO					((UINT16)0xFCC0) // 1.7.0/1.3.7
#define PRISM2_RID_LISTENINTERVAL			((UINT16)0xFCC1) // 1.7.0/1.3.7
#define	PRISM2_RID_SCANREQUEST				(UINT16)0xFCE1)
#define	PRISM2_RID_JOINREQUEST				((UINT16)0xFCE2)
#define	PRISM2_RID_AUTHENTICATESTA			((UINT16)0xFCE3)
#define	PRISM2_RID_CHANNELINFOREQUEST		((UINT16)0xFCE4)
#define	PRISM2_RID_HOSTSCAN          		((UINT16)0xFCE5) // NEW STA

#define	PRISM2_RID_CNFWEPDEFAULTKEY_LEN		((UINT16)6)
#define	PRISM2_RID_CNFWEP128DEFAULTKEY_LEN	((UINT16)14)
#define	PRISM2_RID_CNFPRIOQUSAGE_LEN		((UINT16)4)
