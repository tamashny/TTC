/**************************************************************************************************
 * Copyright (c) 2021 TTControl. All rights reserved. Confidential, proprietary.
 * Schoenbrunnerstrasse 7, A-1040 Wien, Austria. office@ttcontrol.com
 *************************************************************************************************/
/**********************************************************************************************//**
 * \file IO_Pin.h
 *
 * \latexonly \hypertarget{IO_Pin}{} \endlatexonly
 *
 * \brief This header file contains pin definitions for the I/O driver, and aliases for the pins.
 *
 * \verbatim[table]
 * ========================= ===================== =========== ============ =============== =========== =========
 * Connector\n pin           Internal\n pin number \n .        \n .         Pin aliases\n . \n .        \n .
 * ------------------------- --------------------- ----------- ------------ --------------- ----------- ---------
 * \ref IO_PIN_103 "Pin 103"           0           #IO_ADC_00  #IO_DI_48
 * \ref IO_PIN_127 "Pin 127"           1           #IO_ADC_01  #IO_DI_49
 * \ref IO_PIN_104 "Pin 104"           2           #IO_ADC_02  #IO_DI_50
 * \ref IO_PIN_128 "Pin 128"           3           #IO_ADC_03  #IO_DI_51
 * \ref IO_PIN_105 "Pin 105"           4           #IO_ADC_04  #IO_DI_52
 * \ref IO_PIN_129 "Pin 129"           5           #IO_ADC_05  #IO_DI_53
 * \ref IO_PIN_106 "Pin 106"           6           #IO_ADC_06  #IO_DI_54
 * \ref IO_PIN_130 "Pin 130"           7           #IO_ADC_07  #IO_DI_55
 * ------------------------- --------------------- ----------- ------------ --------------- ----------- ---------
 *
 * ------------------------- --------------------- ----------- ------------ --------------- ----------- ---------
 * \ref IO_PIN_107 "Pin 107"           8           #IO_ADC_08  #IO_DI_56
 * \ref IO_PIN_131 "Pin 131"           9           #IO_ADC_09  #IO_DI_57
 * \ref IO_PIN_108 "Pin 108"           10          #IO_ADC_10  #IO_DI_58
 * \ref IO_PIN_132 "Pin 132"           11          #IO_ADC_11  #IO_DI_59
 * \ref IO_PIN_109 "Pin 109"           12          #IO_ADC_12  #IO_DI_60
 * \ref IO_PIN_133 "Pin 133"           13          #IO_ADC_13  #IO_DI_61
 * \ref IO_PIN_110 "Pin 110"           14          #IO_ADC_14  #IO_DI_62
 * \ref IO_PIN_134 "Pin 134"           15          #IO_ADC_15  #IO_DI_63
 * ------------------------- --------------------- ----------- ------------ --------------- ----------- ---------
 *
 * ------------------------- --------------------- ----------- ------------ --------------- ----------- ---------
 * \ref IO_PIN_111 "Pin 111"           16          #IO_ADC_16  #IO_DI_64
 * \ref IO_PIN_135 "Pin 135"           17          #IO_ADC_17  #IO_DI_65
 * \ref IO_PIN_112 "Pin 112"           18          #IO_ADC_18  #IO_DI_66
 * \ref IO_PIN_136 "Pin 136"           19          #IO_ADC_19  #IO_DI_67
 * \ref IO_PIN_113 "Pin 113"           20          #IO_ADC_20  #IO_DI_68
 * \ref IO_PIN_137 "Pin 137"           21          #IO_ADC_21  #IO_DI_69
 * \ref IO_PIN_114 "Pin 114"           22          #IO_ADC_22  #IO_DI_70
 * \ref IO_PIN_138 "Pin 138"           23          #IO_ADC_23  #IO_DI_71
 * ------------------------- --------------------- ----------- ------------ --------------- ----------- ---------
 *
 * ------------------------- --------------------- ----------- ------------ --------------- ----------- ---------
 * \ref IO_PIN_115 "Pin 115"           24          #IO_PWD_00  #IO_ADC_24   #IO_DI_36
 * \ref IO_PIN_139 "Pin 139"           25          #IO_PWD_01  #IO_ADC_25   #IO_DI_37
 * \ref IO_PIN_116 "Pin 116"           26          #IO_PWD_02  #IO_ADC_26   #IO_DI_38
 * \ref IO_PIN_140 "Pin 140"           27          #IO_PWD_03  #IO_ADC_27   #IO_DI_39
 * \ref IO_PIN_117 "Pin 117"           28          #IO_PWD_04  #IO_ADC_28   #IO_DI_40
 * \ref IO_PIN_141 "Pin 141"           29          #IO_PWD_05  #IO_ADC_29   #IO_DI_41
 * ------------------------- --------------------- ----------- ------------ --------------- ----------- ---------
 *
 * ------------------------- --------------------- ----------- ------------ --------------- ----------- ---------
 * \ref IO_PIN_122 "Pin 122"           30          #IO_PWD_06  #IO_ADC_30   #IO_DI_42
 * \ref IO_PIN_146 "Pin 146"           31          #IO_PWD_07  #IO_ADC_31   #IO_DI_43
 * \ref IO_PIN_123 "Pin 123"           32          #IO_PWD_08  #IO_ADC_32   #IO_DI_44
 * \ref IO_PIN_147 "Pin 147"           33          #IO_PWD_09  #IO_ADC_33   #IO_DI_45
 * \ref IO_PIN_124 "Pin 124"           34          #IO_PWD_10  #IO_ADC_34   #IO_DI_46
 * \ref IO_PIN_148 "Pin 148"           35          #IO_PWD_11  #IO_ADC_35   #IO_DI_47
 * ------------------------- --------------------- ----------- ------------ --------------- ----------- ---------
 *
 * ------------------------- --------------------- ----------- ------------ --------------- ----------- ---------
 * \ref IO_PIN_149 "Pin 149"           36          #IO_DO_00   #IO_ADC_36   #IO_DI_72
 * \ref IO_PIN_173 "Pin 173"           37          #IO_DO_01   #IO_ADC_37   #IO_DI_73
 * \ref IO_PIN_152 "Pin 152"           38          #IO_DO_02   #IO_ADC_38   #IO_DI_74
 * \ref IO_PIN_176 "Pin 176"           39          #IO_DO_03   #IO_ADC_39   #IO_DI_75
 * \ref IO_PIN_155 "Pin 155"           40          #IO_DO_04   #IO_ADC_40   #IO_DI_76
 * \ref IO_PIN_179 "Pin 179"           41          #IO_DO_05   #IO_ADC_41   #IO_DI_77
 * \ref IO_PIN_158 "Pin 158"           42          #IO_DO_06   #IO_ADC_42   #IO_DI_78
 * \ref IO_PIN_182 "Pin 182"           43          #IO_DO_07   #IO_ADC_43   #IO_DI_79
 * ------------------------- --------------------- ----------- ------------ --------------- ----------- ---------
 *
 * ------------------------- --------------------- ----------- ------------ --------------- ----------- ---------
 * \ref IO_PIN_251 "Pin 251"           44          #IO_DO_08   #IO_ADC_44   #IO_DI_80
 * \ref IO_PIN_238 "Pin 238"           45          #IO_DO_09   #IO_ADC_45   #IO_DI_81
 * \ref IO_PIN_252 "Pin 252"           46          #IO_DO_10   #IO_ADC_46   #IO_DI_82
 * \ref IO_PIN_239 "Pin 239"           47          #IO_DO_11   #IO_ADC_47   #IO_DI_83
 * \ref IO_PIN_253 "Pin 253"           48          #IO_DO_12   #IO_ADC_48   #IO_DI_84
 * \ref IO_PIN_240 "Pin 240"           49          #IO_DO_13   #IO_ADC_49   #IO_DI_85
 * \ref IO_PIN_254 "Pin 254"           50          #IO_DO_14   #IO_ADC_50   #IO_DI_86
 * \ref IO_PIN_241 "Pin 241"           51          #IO_DO_15   #IO_ADC_51   #IO_DI_87
 * ------------------------- --------------------- ----------- ------------ --------------- ----------- ---------
 *
 * ------------------------- --------------------- ----------- ------------ --------------- ----------- ---------
 * \ref IO_PIN_153 "Pin 153"           52          #IO_PWM_00  #IO_DO_16    #IO_DI_00
 * \ref IO_PIN_177 "Pin 177"           53          #IO_PWM_01  #IO_DO_17    #IO_DI_01
 * \ref IO_PIN_156 "Pin 156"           54          #IO_PWM_02  #IO_DO_18    #IO_DI_02
 * \ref IO_PIN_180 "Pin 180"           55          #IO_PWM_03  #IO_DO_19    #IO_DI_03
 * \ref IO_PIN_159 "Pin 159"           56          #IO_PWM_04  #IO_DO_20    #IO_DI_04
 * \ref IO_PIN_183 "Pin 183"           57          #IO_PWM_05  #IO_DO_21    #IO_DI_05
 * \ref IO_PIN_186 "Pin 186"           58          #IO_PWM_06  #IO_DO_22    #IO_DI_06
 * \ref IO_PIN_162 "Pin 162"           59          #IO_PWM_07  #IO_DO_23    #IO_DI_07
 * \ref IO_PIN_189 "Pin 189"           60          #IO_PWM_08  #IO_DO_24    #IO_DI_08
 * \ref IO_PIN_165 "Pin 165"           61          #IO_PWM_09  #IO_DO_25    #IO_DI_09
 * \ref IO_PIN_192 "Pin 192"           62          #IO_PWM_10  #IO_DO_26    #IO_DI_10
 * \ref IO_PIN_168 "Pin 168"           63          #IO_PWM_11  #IO_DO_27    #IO_DI_11
 * \ref IO_PIN_195 "Pin 195"           64          #IO_PWM_12  #IO_DO_28    #IO_DI_12
 * \ref IO_PIN_171 "Pin 171"           65          #IO_PWM_13  #IO_DO_29    #IO_DI_13
 * ------------------------- --------------------- ----------- ------------ --------------- ----------- ---------
 *
 * ------------------------- --------------------- ----------- ------------ --------------- ----------- ---------
 * \ref IO_PIN_154 "Pin 154"           66          #IO_PWM_14  #IO_DO_30    #IO_DI_14
 * \ref IO_PIN_178 "Pin 178"           67          #IO_PWM_15  #IO_DO_31    #IO_DI_15
 * \ref IO_PIN_157 "Pin 157"           68          #IO_PWM_16  #IO_DO_32    #IO_DI_16
 * \ref IO_PIN_181 "Pin 181"           69          #IO_PWM_17  #IO_DO_33    #IO_DI_17
 * \ref IO_PIN_160 "Pin 160"           70          #IO_PWM_18  #IO_DO_34    #IO_DI_18
 * \ref IO_PIN_184 "Pin 184"           71          #IO_PWM_19  #IO_DO_35    #IO_DI_19
 * \ref IO_PIN_187 "Pin 187"           72          #IO_PWM_20  #IO_DO_36    #IO_DI_20
 * \ref IO_PIN_163 "Pin 163"           73          #IO_PWM_21  #IO_DO_37    #IO_DI_21
 * \ref IO_PIN_190 "Pin 190"           74          #IO_PWM_22  #IO_DO_38    #IO_DI_22
 * \ref IO_PIN_166 "Pin 166"           75          #IO_PWM_23  #IO_DO_39    #IO_DI_23
 * \ref IO_PIN_193 "Pin 193"           76          #IO_PWM_24  #IO_DO_40    #IO_DI_24
 * \ref IO_PIN_169 "Pin 169"           77          #IO_PWM_25  #IO_DO_41    #IO_DI_25
 * \ref IO_PIN_196 "Pin 196"           78          #IO_PWM_26  #IO_DO_42    #IO_DI_26
 * \ref IO_PIN_172 "Pin 172"           79          #IO_PWM_27  #IO_DO_43    #IO_DI_27
 * ------------------------- --------------------- ----------- ------------ --------------- ----------- ---------
 *
 * ------------------------- --------------------- ----------- ------------ --------------- ----------- ---------
 * \ref IO_PIN_101 "Pin 101"           80          #IO_PWM_28  #IO_DO_44    #IO_DI_28       #IO_PWD_12
 * \ref IO_PIN_125 "Pin 125"           81          #IO_PWM_29  #IO_DO_45    #IO_DI_29       #IO_PWD_13
 * \ref IO_PIN_150 "Pin 150"           82          #IO_PWM_30  #IO_DO_46    #IO_DI_30       #IO_PWD_14
 * \ref IO_PIN_174 "Pin 174"           83          #IO_PWM_31  #IO_DO_47    #IO_DI_31       #IO_PWD_15
 * \ref IO_PIN_102 "Pin 102"           84          #IO_PWM_32  #IO_DO_48    #IO_DI_32       #IO_PWD_16
 * \ref IO_PIN_126 "Pin 126"           85          #IO_PWM_33  #IO_DO_49    #IO_DI_33       #IO_PWD_17
 * \ref IO_PIN_151 "Pin 151"           86          #IO_PWM_34  #IO_DO_50    #IO_DI_34       #IO_PWD_18
 * \ref IO_PIN_175 "Pin 175"           87          #IO_PWM_35  #IO_DO_51    #IO_DI_35       #IO_PWD_19
 * ------------------------- --------------------- ----------- ------------ --------------- ----------- ---------
 *
 * ------------------------- --------------------- ----------- ------------ --------------- ----------- ---------
 * \ref IO_PIN_161 "Pin 161"           88          #IO_PVG_00  #IO_VOUT_00  #IO_DO_52       #IO_ADC_52  #IO_DI_88
 * \ref IO_PIN_185 "Pin 185"           89          #IO_PVG_01  #IO_VOUT_01  #IO_DO_53       #IO_ADC_53  #IO_DI_89
 * \ref IO_PIN_188 "Pin 188"           90          #IO_PVG_02  #IO_VOUT_02  #IO_DO_54       #IO_ADC_54  #IO_DI_90
 * \ref IO_PIN_164 "Pin 164"           91          #IO_PVG_03  #IO_VOUT_03  #IO_DO_55       #IO_ADC_55  #IO_DI_91
 * \ref IO_PIN_191 "Pin 191"           92          #IO_PVG_04  #IO_VOUT_04  #IO_DO_56       #IO_ADC_56  #IO_DI_92
 * \ref IO_PIN_167 "Pin 167"           93          #IO_PVG_05  #IO_VOUT_05  #IO_DO_57       #IO_ADC_57  #IO_DI_93
 * \ref IO_PIN_194 "Pin 194"           94          #IO_PVG_06  #IO_VOUT_06  #IO_DO_58       #IO_ADC_58  #IO_DI_94
 * \ref IO_PIN_170 "Pin 170"           95          #IO_PVG_07  #IO_VOUT_07  #IO_DO_59       #IO_ADC_59  #IO_DI_95
 * ========================= ===================== =========== ============ =============== =========== =========

 * \endverbatim
 *
 *************************************************************************************************/

#ifndef IO_PIN_H_
#define IO_PIN_H_

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
 *
 * D E F I N I T I O N S
 *
 *************************************************************************************************/

/*
 * P I N   D E F I N I T I O N S
 */

/**
 * \name Connector Pins
 * \anchor io_pins
 *
 * Defines for the Connector Pins.
 *
 * @{
 */
#define IO_PIN_101      80U      /**< Pin Nr. 101 */
#define IO_PIN_102      84U      /**< Pin Nr. 102 */
#define IO_PIN_103       0U      /**< Pin Nr. 103 */
#define IO_PIN_104       2U      /**< Pin Nr. 104 */
#define IO_PIN_105       4U      /**< Pin Nr. 105 */
#define IO_PIN_106       6U      /**< Pin Nr. 106 */
#define IO_PIN_107       8U      /**< Pin Nr. 107 */
#define IO_PIN_108      10U      /**< Pin Nr. 108 */
#define IO_PIN_109      12U      /**< Pin Nr. 109 */
#define IO_PIN_110      14U      /**< Pin Nr. 110 */
#define IO_PIN_111      16U      /**< Pin Nr. 111 */
#define IO_PIN_112      18U      /**< Pin Nr. 112 */
#define IO_PIN_113      20U      /**< Pin Nr. 113 */
#define IO_PIN_114      22U      /**< Pin Nr. 114 */
#define IO_PIN_115      24U      /**< Pin Nr. 115 */
#define IO_PIN_116      26U      /**< Pin Nr. 116 */
#define IO_PIN_117      28U      /**< Pin Nr. 117 */
#define IO_PIN_122      30U      /**< Pin Nr. 122 */
#define IO_PIN_123      32U      /**< Pin Nr. 123 */
#define IO_PIN_124      34U      /**< Pin Nr. 124 */
#define IO_PIN_125      81U      /**< Pin Nr. 125 */
#define IO_PIN_126      85U      /**< Pin Nr. 126 */
#define IO_PIN_127       1U      /**< Pin Nr. 127 */
#define IO_PIN_128       3U      /**< Pin Nr. 128 */
#define IO_PIN_129       5U      /**< Pin Nr. 129 */
#define IO_PIN_130       7U      /**< Pin Nr. 130 */
#define IO_PIN_131       9U      /**< Pin Nr. 131 */
#define IO_PIN_132      11U      /**< Pin Nr. 132 */
#define IO_PIN_133      13U      /**< Pin Nr. 133 */
#define IO_PIN_134      15U      /**< Pin Nr. 134 */
#define IO_PIN_135      17U      /**< Pin Nr. 135 */
#define IO_PIN_136      19U      /**< Pin Nr. 136 */
#define IO_PIN_137      21U      /**< Pin Nr. 137 */
#define IO_PIN_138      23U      /**< Pin Nr. 138 */
#define IO_PIN_139      25U      /**< Pin Nr. 139 */
#define IO_PIN_140      27U      /**< Pin Nr. 140 */
#define IO_PIN_141      29U      /**< Pin Nr. 141 */
#define IO_PIN_146      31U      /**< Pin Nr. 146 */
#define IO_PIN_147      33U      /**< Pin Nr. 147 */
#define IO_PIN_148      35U      /**< Pin Nr. 148 */
#define IO_PIN_149      36U      /**< Pin Nr. 149 */
#define IO_PIN_150      82U      /**< Pin Nr. 150 */
#define IO_PIN_151      86U      /**< Pin Nr. 151 */
#define IO_PIN_152      38U      /**< Pin Nr. 152 */
#define IO_PIN_153      52U      /**< Pin Nr. 153 */
#define IO_PIN_154      66U      /**< Pin Nr. 154 */
#define IO_PIN_155      40U      /**< Pin Nr. 155 */
#define IO_PIN_156      54U      /**< Pin Nr. 156 */
#define IO_PIN_157      68U      /**< Pin Nr. 157 */
#define IO_PIN_158      42U      /**< Pin Nr. 158 */
#define IO_PIN_159      56U      /**< Pin Nr. 159 */
#define IO_PIN_160      70U      /**< Pin Nr. 160 */
#define IO_PIN_161      88U      /**< Pin Nr. 161 */
#define IO_PIN_162      59U      /**< Pin Nr. 162 */
#define IO_PIN_163      73U      /**< Pin Nr. 163 */
#define IO_PIN_164      91U      /**< Pin Nr. 164 */
#define IO_PIN_165      61U      /**< Pin Nr. 165 */
#define IO_PIN_166      75U      /**< Pin Nr. 166 */
#define IO_PIN_167      93U      /**< Pin Nr. 167 */
#define IO_PIN_168      63U      /**< Pin Nr. 168 */
#define IO_PIN_169      77U      /**< Pin Nr. 169 */
#define IO_PIN_170      95U      /**< Pin Nr. 170 */
#define IO_PIN_171      65U      /**< Pin Nr. 171 */
#define IO_PIN_172      79U      /**< Pin Nr. 172 */
#define IO_PIN_173      37U      /**< Pin Nr. 173 */
#define IO_PIN_174      83U      /**< Pin Nr. 174 */
#define IO_PIN_175      87U      /**< Pin Nr. 175 */
#define IO_PIN_176      39U      /**< Pin Nr. 176 */
#define IO_PIN_177      53U      /**< Pin Nr. 177 */
#define IO_PIN_178      67U      /**< Pin Nr. 178 */
#define IO_PIN_179      41U      /**< Pin Nr. 179 */
#define IO_PIN_180      55U      /**< Pin Nr. 180 */
#define IO_PIN_181      69U      /**< Pin Nr. 181 */
#define IO_PIN_182      43U      /**< Pin Nr. 182 */
#define IO_PIN_183      57U      /**< Pin Nr. 183 */
#define IO_PIN_184      71U      /**< Pin Nr. 184 */
#define IO_PIN_185      89U      /**< Pin Nr. 185 */
#define IO_PIN_186      58U      /**< Pin Nr. 186 */
#define IO_PIN_187      72U      /**< Pin Nr. 187 */
#define IO_PIN_188      90U      /**< Pin Nr. 188 */
#define IO_PIN_189      60U      /**< Pin Nr. 189 */
#define IO_PIN_190      74U      /**< Pin Nr. 190 */
#define IO_PIN_191      92U      /**< Pin Nr. 191 */
#define IO_PIN_192      62U      /**< Pin Nr. 192 */
#define IO_PIN_193      76U      /**< Pin Nr. 193 */
#define IO_PIN_194      94U      /**< Pin Nr. 194 */
#define IO_PIN_195      64U      /**< Pin Nr. 195 */
#define IO_PIN_196      78U      /**< Pin Nr. 196 */
#define IO_PIN_207      97U      /**< Pin Nr. 207 */
#define IO_PIN_220      98U      /**< Pin Nr. 220 */
#define IO_PIN_221     101U      /**< Pin Nr. 221 */
#define IO_PIN_234     100U      /**< Pin Nr. 234 */
#define IO_PIN_238      45U      /**< Pin Nr. 238 */
#define IO_PIN_239      47U      /**< Pin Nr. 239 */
#define IO_PIN_240      49U      /**< Pin Nr. 240 */
#define IO_PIN_241      51U      /**< Pin Nr. 241 */
#define IO_PIN_246      96U      /**< Pin Nr. 246 */
#define IO_PIN_247      99U      /**< Pin Nr. 247 */
#define IO_PIN_251      44U      /**< Pin Nr. 251 */
#define IO_PIN_252      46U      /**< Pin Nr. 252 */
#define IO_PIN_253      48U      /**< Pin Nr. 253 */
#define IO_PIN_254      50U      /**< Pin Nr. 254 */
#define IO_PIN_118     253U      /**< Pin Nr. 118 (BAT-, ground)  */
#define IO_PIN_201     254U      /**< Pin Nr. 201 (BAT+ Power)    */
#define IO_PIN_NONE    255U      /**< Undefined pin               */
/** @} */

/**
 * \name Internal Pins
 *
 * These Pins are not available on the ECU Connector.
 * They are needed for internal control or monitoring.
 *
 * @{
 */
#define IO_INT_PIN_CAN_CH0          102U  /**< internal for CAN Channel0          */
#define IO_INT_PIN_CAN_CH1          103U  /**< internal for CAN Channel1          */
#define IO_INT_PIN_CAN_CH2          104U  /**< internal for CAN Channel2          */
#define IO_INT_PIN_CAN_CH3          105U  /**< internal for CAN Channel3          */
#define IO_INT_PIN_CAN_CH4          106U  /**< internal for CAN Channel4          */
#define IO_INT_PIN_CAN_CH5          107U  /**< internal for CAN Channel5          */
#define IO_INT_PIN_CAN_CH6          108U  /**< internal for CAN Channel6          */
#define IO_INT_PIN_SAFETY_SW_0      109U  /**< internal Pin for Safety Switch 0, see
                                \ref IO_POWER.h "Driver for ECU Power functions" for details */
#define IO_INT_PIN_SAFETY_SW_1      110U  /**< internal Pin for Safety Switch 1, see
                                \ref IO_POWER.h "Driver for ECU Power functions" for details */
#define IO_INT_PIN_SAFETY_SW_2      111U  /**< internal Pin for Safety Switch 2, see
                                \ref IO_POWER.h "Driver for ECU Power functions" for details */
#define IO_INT_PIN_SAFETY_SW_VP     112U  /**< internal Pin for Safety Switch VP  */
#define IO_INT_PIN_ENDRV_CPU        113U  /**< internal Pin for enable drive CPU, see
                                \ref IO_POWER.h "Driver for ECU Power functions" for details */
#define IO_INT_PIN_PWD              117U  /**< internal for PWD module            */
#define IO_INT_PIN_TEMP             120U  /**< internal for board temperature     */
#define IO_INT_PIN_REF_2V5          121U  /**< internal 2.5V reference voltage    */
#define IO_INT_PIN_1V2              122U  /**< internal 1.2V supply voltage       */
#define IO_INT_PIN_VMON             123U  /**< internal for voltage monitor       */
/** @} */


/*
 * A L I A S E S   F O R   T H E   A B O V E   P I N   D E F I N I T I O N S
 */

/**
 * \name Pin Aliases
 *
 * Aliases for the pin definitions based on their function.
 *
 * @{
 */
 
/* 3 mode */
#define IO_ADC_00                   IO_PIN_103   /**< main function 3 mode ADC input */
#define IO_ADC_01                   IO_PIN_127   /**< main function 3 mode ADC input */
#define IO_ADC_02                   IO_PIN_104   /**< main function 3 mode ADC input */
#define IO_ADC_03                   IO_PIN_128   /**< main function 3 mode ADC input */
#define IO_ADC_04                   IO_PIN_105   /**< main function 3 mode ADC input */
#define IO_ADC_05                   IO_PIN_129   /**< main function 3 mode ADC input */
#define IO_ADC_06                   IO_PIN_106   /**< main function 3 mode ADC input */
#define IO_ADC_07                   IO_PIN_130   /**< main function 3 mode ADC input */

/* 2 mode 10V */
#define IO_ADC_08                   IO_PIN_107   /**< main function 2 mode 10V ADC input */
#define IO_ADC_09                   IO_PIN_131   /**< main function 2 mode 10V ADC input */
#define IO_ADC_10                   IO_PIN_108   /**< main function 2 mode 10V ADC input */
#define IO_ADC_11                   IO_PIN_132   /**< main function 2 mode 10V ADC input */
#define IO_ADC_12                   IO_PIN_109   /**< main function 2 mode 10V ADC input */
#define IO_ADC_13                   IO_PIN_133   /**< main function 2 mode 10V ADC input */
#define IO_ADC_14                   IO_PIN_110   /**< main function 2 mode 10V ADC input */
#define IO_ADC_15                   IO_PIN_134   /**< main function 2 mode 10V ADC input */

/* 2 mode 32V */
#define IO_ADC_16                   IO_PIN_111   /**< main function 2 mode 32V ADC input */
#define IO_ADC_17                   IO_PIN_135   /**< main function 2 mode 32V ADC input */
#define IO_ADC_18                   IO_PIN_112   /**< main function 2 mode 32V ADC input */
#define IO_ADC_19                   IO_PIN_136   /**< main function 2 mode 32V ADC input */
#define IO_ADC_20                   IO_PIN_113   /**< main function 2 mode 32V ADC input */
#define IO_ADC_21                   IO_PIN_137   /**< main function 2 mode 32V ADC input */
#define IO_ADC_22                   IO_PIN_114   /**< main function 2 mode 32V ADC input */
#define IO_ADC_23                   IO_PIN_138   /**< main function 2 mode 32V ADC input */

#define IO_ADC_24                   IO_PIN_115   /**< alternative ADC function for #IO_PWD_00 */
#define IO_ADC_25                   IO_PIN_139   /**< alternative ADC function for #IO_PWD_01 */
#define IO_ADC_26                   IO_PIN_116   /**< alternative ADC function for #IO_PWD_02 */
#define IO_ADC_27                   IO_PIN_140   /**< alternative ADC function for #IO_PWD_03 */
#define IO_ADC_28                   IO_PIN_117   /**< alternative ADC function for #IO_PWD_04 */
#define IO_ADC_29                   IO_PIN_141   /**< alternative ADC function for #IO_PWD_05 */
#define IO_ADC_30                   IO_PIN_122   /**< alternative ADC function for #IO_PWD_06 */
#define IO_ADC_31                   IO_PIN_146   /**< alternative ADC function for #IO_PWD_07 */
#define IO_ADC_32                   IO_PIN_123   /**< alternative ADC function for #IO_PWD_08 */
#define IO_ADC_33                   IO_PIN_147   /**< alternative ADC function for #IO_PWD_09 */
#define IO_ADC_34                   IO_PIN_124   /**< alternative ADC function for #IO_PWD_10 */
#define IO_ADC_35                   IO_PIN_148   /**< alternative ADC function for #IO_PWD_11 */

#define IO_ADC_36                   IO_PIN_149   /**< alternative ADC function for #IO_DO_00 */
#define IO_ADC_37                   IO_PIN_173   /**< alternative ADC function for #IO_DO_01 */
#define IO_ADC_38                   IO_PIN_152   /**< alternative ADC function for #IO_DO_02 */
#define IO_ADC_39                   IO_PIN_176   /**< alternative ADC function for #IO_DO_03 */
#define IO_ADC_40                   IO_PIN_155   /**< alternative ADC function for #IO_DO_04 */
#define IO_ADC_41                   IO_PIN_179   /**< alternative ADC function for #IO_DO_05 */
#define IO_ADC_42                   IO_PIN_158   /**< alternative ADC function for #IO_DO_06 */
#define IO_ADC_43                   IO_PIN_182   /**< alternative ADC function for #IO_DO_07 */

#define IO_ADC_44                   IO_PIN_251   /**< alternative ADC function for #IO_DO_08 */
#define IO_ADC_45                   IO_PIN_238   /**< alternative ADC function for #IO_DO_09 */
#define IO_ADC_46                   IO_PIN_252   /**< alternative ADC function for #IO_DO_10 */
#define IO_ADC_47                   IO_PIN_239   /**< alternative ADC function for #IO_DO_11 */
#define IO_ADC_48                   IO_PIN_253   /**< alternative ADC function for #IO_DO_12 */
#define IO_ADC_49                   IO_PIN_240   /**< alternative ADC function for #IO_DO_13 */
#define IO_ADC_50                   IO_PIN_254   /**< alternative ADC function for #IO_DO_14 */
#define IO_ADC_51                   IO_PIN_241   /**< alternative ADC function for #IO_DO_15 */

#define IO_ADC_52                   IO_PIN_161   /**< alternative ADC function for #IO_PVG_00 */
#define IO_ADC_53                   IO_PIN_185   /**< alternative ADC function for #IO_PVG_01 */
#define IO_ADC_54                   IO_PIN_188   /**< alternative ADC function for #IO_PVG_02 */
#define IO_ADC_55                   IO_PIN_164   /**< alternative ADC function for #IO_PVG_03 */
#define IO_ADC_56                   IO_PIN_191   /**< alternative ADC function for #IO_PVG_04 */
#define IO_ADC_57                   IO_PIN_167   /**< alternative ADC function for #IO_PVG_05 */
#define IO_ADC_58                   IO_PIN_194   /**< alternative ADC function for #IO_PVG_06 */
#define IO_ADC_59                   IO_PIN_170   /**< alternative ADC function for #IO_PVG_07 */

/* other ADC */
#define IO_ADC_UBAT                 IO_PIN_246   /**< This ADC channel can be used to monitor
                                                      the voltage on connector pin K30-A,
                                                      i.e., the BAT+ CPU supply. Note that
                                                      connector pins K30-P, i.e., BAT+, cannot
                                                      be monitored at application level!      */
#define IO_ADC_K15                  IO_K15
#define IO_ADC_WAKE_UP              IO_WAKEUP
#define IO_ADC_SENSOR_SUPPLY_0      IO_SENSOR_SUPPLY_0
#define IO_ADC_SENSOR_SUPPLY_1      IO_SENSOR_SUPPLY_1
#define IO_ADC_SENSOR_SUPPLY_2      IO_SENSOR_SUPPLY_2
#define IO_ADC_SAFETY_SW_0          IO_INT_SAFETY_SW_0
#define IO_ADC_SAFETY_SW_1          IO_INT_SAFETY_SW_1
#define IO_ADC_SAFETY_SW_2          IO_INT_SAFETY_SW_2
#define IO_ADC_BOARD_TEMP           IO_INT_PIN_TEMP

/* sensor supplies */
#define IO_SENSOR_SUPPLY_0          IO_PIN_247
#define IO_SENSOR_SUPPLY_1          IO_PIN_234
#define IO_SENSOR_SUPPLY_2          IO_PIN_221

/* K15 and wake up */
#define IO_K15                      IO_PIN_207
#define IO_WAKEUP                   IO_PIN_220

#define IO_GND                      IO_PIN_118
#define IO_UBAT                     IO_PIN_201

/* PWD */
#define IO_PWD_00                   IO_PIN_115 /**< main function timer input */
#define IO_PWD_01                   IO_PIN_139 /**< main function timer input */
#define IO_PWD_02                   IO_PIN_116 /**< main function timer input */
#define IO_PWD_03                   IO_PIN_140 /**< main function timer input */
#define IO_PWD_04                   IO_PIN_117 /**< main function timer input */
#define IO_PWD_05                   IO_PIN_141 /**< main function timer input */
#define IO_PWD_06                   IO_PIN_122 /**< main function timer input */
#define IO_PWD_07                   IO_PIN_146 /**< main function timer input */
#define IO_PWD_08                   IO_PIN_123 /**< main function timer input */
#define IO_PWD_09                   IO_PIN_147 /**< main function timer input */
#define IO_PWD_10                   IO_PIN_124 /**< main function timer input */
#define IO_PWD_11                   IO_PIN_148 /**< main function timer input */
#define IO_PWD_12                   IO_PIN_101 /**< alternative timer function for #IO_PWM_28 */
#define IO_PWD_13                   IO_PIN_125 /**< alternative timer function for #IO_PWM_29 */
#define IO_PWD_14                   IO_PIN_150 /**< alternative timer function for #IO_PWM_30 */
#define IO_PWD_15                   IO_PIN_174 /**< alternative timer function for #IO_PWM_31 */
#define IO_PWD_16                   IO_PIN_102 /**< alternative timer function for #IO_PWM_32 */
#define IO_PWD_17                   IO_PIN_126 /**< alternative timer function for #IO_PWM_33 */
#define IO_PWD_18                   IO_PIN_151 /**< alternative timer function for #IO_PWM_34 */
#define IO_PWD_19                   IO_PIN_175 /**< alternative timer function for #IO_PWM_35 */

/* PWM */
#define IO_PWM_00                   IO_PIN_153 /**< main function PWM output (shut off group 0) */
#define IO_PWM_01                   IO_PIN_177 /**< main function PWM output (shut off group 0) */
#define IO_PWM_02                   IO_PIN_156 /**< main function PWM output (shut off group 0) */
#define IO_PWM_03                   IO_PIN_180 /**< main function PWM output (shut off group 0) */
#define IO_PWM_04                   IO_PIN_159 /**< main function PWM output (shut off group 0) */
#define IO_PWM_05                   IO_PIN_183 /**< main function PWM output (shut off group 0) */
#define IO_PWM_06                   IO_PIN_186 /**< main function PWM output (shut off group 0) */
#define IO_PWM_07                   IO_PIN_162 /**< main function PWM output (shut off group 0) */
#define IO_PWM_08                   IO_PIN_189 /**< main function PWM output (shut off group 0) */
#define IO_PWM_09                   IO_PIN_165 /**< main function PWM output (shut off group 0) */
#define IO_PWM_10                   IO_PIN_192 /**< main function PWM output (shut off group 0) */
#define IO_PWM_11                   IO_PIN_168 /**< main function PWM output (shut off group 0) */
#define IO_PWM_12                   IO_PIN_195 /**< main function PWM output (shut off group 0) */
#define IO_PWM_13                   IO_PIN_171 /**< main function PWM output (shut off group 0) */
#define IO_PWM_14                   IO_PIN_154 /**< main function PWM output (shut off group 1) */
#define IO_PWM_15                   IO_PIN_178 /**< main function PWM output (shut off group 1) */
#define IO_PWM_16                   IO_PIN_157 /**< main function PWM output (shut off group 1) */
#define IO_PWM_17                   IO_PIN_181 /**< main function PWM output (shut off group 1) */
#define IO_PWM_18                   IO_PIN_160 /**< main function PWM output (shut off group 1) */
#define IO_PWM_19                   IO_PIN_184 /**< main function PWM output (shut off group 1) */
#define IO_PWM_20                   IO_PIN_187 /**< main function PWM output (shut off group 1) */
#define IO_PWM_21                   IO_PIN_163 /**< main function PWM output (shut off group 1) */
#define IO_PWM_22                   IO_PIN_190 /**< main function PWM output (shut off group 1) */
#define IO_PWM_23                   IO_PIN_166 /**< main function PWM output (shut off group 1) */
#define IO_PWM_24                   IO_PIN_193 /**< main function PWM output (shut off group 1) */
#define IO_PWM_25                   IO_PIN_169 /**< main function PWM output (shut off group 1) */
#define IO_PWM_26                   IO_PIN_196 /**< main function PWM output (shut off group 1) */
#define IO_PWM_27                   IO_PIN_172 /**< main function PWM output (shut off group 1) */
#define IO_PWM_28                   IO_PIN_101 /**< main function PWM output (shut off group 2) */
#define IO_PWM_29                   IO_PIN_125 /**< main function PWM output (shut off group 2) */
#define IO_PWM_30                   IO_PIN_150 /**< main function PWM output (shut off group 2) */
#define IO_PWM_31                   IO_PIN_174 /**< main function PWM output (shut off group 2) */
#define IO_PWM_32                   IO_PIN_102 /**< main function PWM output (shut off group 2) */
#define IO_PWM_33                   IO_PIN_126 /**< main function PWM output (shut off group 2) */
#define IO_PWM_34                   IO_PIN_151 /**< main function PWM output (shut off group 2) */
#define IO_PWM_35                   IO_PIN_175 /**< main function PWM output (shut off group 2) */

/* PVG */
#define IO_PVG_00                   IO_PIN_161 /**< main function PVG output */
#define IO_PVG_01                   IO_PIN_185 /**< main function PVG output */
#define IO_PVG_02                   IO_PIN_188 /**< main function PVG output */
#define IO_PVG_03                   IO_PIN_164 /**< main function PVG output */
#define IO_PVG_04                   IO_PIN_191 /**< main function PVG output */
#define IO_PVG_05                   IO_PIN_167 /**< main function PVG output */
#define IO_PVG_06                   IO_PIN_194 /**< main function PVG output */
#define IO_PVG_07                   IO_PIN_170 /**< main function PVG output */

/* VOUT */
#define IO_VOUT_00                  IO_PIN_161 /**< alternative voltage output function for
                                                    #IO_PVG_00 */
#define IO_VOUT_01                  IO_PIN_185 /**< alternative voltage output function for
                                                    #IO_PVG_01 */
#define IO_VOUT_02                  IO_PIN_188 /**< alternative voltage output function for
                                                    #IO_PVG_02 */
#define IO_VOUT_03                  IO_PIN_164 /**< alternative voltage output function for
                                                    #IO_PVG_03 */
#define IO_VOUT_04                  IO_PIN_191 /**< alternative voltage output function for
                                                    #IO_PVG_04 */
#define IO_VOUT_05                  IO_PIN_167 /**< alternative voltage output function for
                                                    #IO_PVG_05 */
#define IO_VOUT_06                  IO_PIN_194 /**< alternative voltage output function for
                                                    #IO_PVG_06 */
#define IO_VOUT_07                  IO_PIN_170 /**< alternative voltage output function for
                                                    #IO_PVG_07 */

/* Digital inputs */
#define IO_DI_00                    IO_PIN_153 /**< alternative digital input function for
                                                    #IO_PWM_00 */
#define IO_DI_01                    IO_PIN_177 /**< alternative digital input function for
                                                    #IO_PWM_01 */
#define IO_DI_02                    IO_PIN_156 /**< alternative digital input function for
                                                    #IO_PWM_02 */
#define IO_DI_03                    IO_PIN_180 /**< alternative digital input function for
                                                    #IO_PWM_03 */
#define IO_DI_04                    IO_PIN_159 /**< alternative digital input function for
                                                    #IO_PWM_04 */
#define IO_DI_05                    IO_PIN_183 /**< alternative digital input function for
                                                    #IO_PWM_05 */
#define IO_DI_06                    IO_PIN_186 /**< alternative digital input function for
                                                    #IO_PWM_06 */
#define IO_DI_07                    IO_PIN_162 /**< alternative digital input function for
                                                    #IO_PWM_07 */
#define IO_DI_08                    IO_PIN_189 /**< alternative digital input function for
                                                    #IO_PWM_08 */
#define IO_DI_09                    IO_PIN_165 /**< alternative digital input function for
                                                    #IO_PWM_09 */
#define IO_DI_10                    IO_PIN_192 /**< alternative digital input function for
                                                    #IO_PWM_10 */
#define IO_DI_11                    IO_PIN_168 /**< alternative digital input function for
                                                    #IO_PWM_11 */
#define IO_DI_12                    IO_PIN_195 /**< alternative digital input function for
                                                    #IO_PWM_12 */
#define IO_DI_13                    IO_PIN_171 /**< alternative digital input function for
                                                    #IO_PWM_13 */
#define IO_DI_14                    IO_PIN_154 /**< alternative digital input function for
                                                    #IO_PWM_14 */
#define IO_DI_15                    IO_PIN_178 /**< alternative digital input function for
                                                    #IO_PWM_15 */
#define IO_DI_16                    IO_PIN_157 /**< alternative digital input function for
                                                    #IO_PWM_16 */
#define IO_DI_17                    IO_PIN_181 /**< alternative digital input function for
                                                    #IO_PWM_17 */
#define IO_DI_18                    IO_PIN_160 /**< alternative digital input function for
                                                    #IO_PWM_18 */
#define IO_DI_19                    IO_PIN_184 /**< alternative digital input function for
                                                    #IO_PWM_19 */
#define IO_DI_20                    IO_PIN_187 /**< alternative digital input function for
                                                    #IO_PWM_20 */
#define IO_DI_21                    IO_PIN_163 /**< alternative digital input function for
                                                    #IO_PWM_21 */
#define IO_DI_22                    IO_PIN_190 /**< alternative digital input function for
                                                    #IO_PWM_22 */
#define IO_DI_23                    IO_PIN_166 /**< alternative digital input function for
                                                    #IO_PWM_23 */
#define IO_DI_24                    IO_PIN_193 /**< alternative digital input function for
                                                    #IO_PWM_24 */
#define IO_DI_25                    IO_PIN_169 /**< alternative digital input function for
                                                    #IO_PWM_25 */
#define IO_DI_26                    IO_PIN_196 /**< alternative digital input function for
                                                    #IO_PWM_26 */
#define IO_DI_27                    IO_PIN_172 /**< alternative digital input function for
                                                    #IO_PWM_27 */
#define IO_DI_28                    IO_PIN_101 /**< alternative digital input function for
                                                    #IO_PWM_28 */
#define IO_DI_29                    IO_PIN_125 /**< alternative digital input function for
                                                    #IO_PWM_29 */
#define IO_DI_30                    IO_PIN_150 /**< alternative digital input function for
                                                    #IO_PWM_30 */
#define IO_DI_31                    IO_PIN_174 /**< alternative digital input function for
                                                    #IO_PWM_31 */
#define IO_DI_32                    IO_PIN_102 /**< alternative digital input function for
                                                    #IO_PWM_32 */
#define IO_DI_33                    IO_PIN_126 /**< alternative digital input function for
                                                    #IO_PWM_33 */
#define IO_DI_34                    IO_PIN_151 /**< alternative digital input function for
                                                    #IO_PWM_34 */
#define IO_DI_35                    IO_PIN_175 /**< alternative digital input function for
                                                    #IO_PWM_35 */

#define IO_DI_36                    IO_PIN_115 /**< alternative digital input function for
                                                    #IO_PWD_00 */
#define IO_DI_37                    IO_PIN_139 /**< alternative digital input function for
                                                    #IO_PWD_01 */
#define IO_DI_38                    IO_PIN_116 /**< alternative digital input function for
                                                    #IO_PWD_02 */
#define IO_DI_39                    IO_PIN_140 /**< alternative digital input function for
                                                    #IO_PWD_03 */
#define IO_DI_40                    IO_PIN_117 /**< alternative digital input function for
                                                    #IO_PWD_04 */
#define IO_DI_41                    IO_PIN_141 /**< alternative digital input function for
                                                    #IO_PWD_05 */
#define IO_DI_42                    IO_PIN_122 /**< alternative digital input function for
                                                    #IO_PWD_06 */
#define IO_DI_43                    IO_PIN_146 /**< alternative digital input function for
                                                    #IO_PWD_07 */
#define IO_DI_44                    IO_PIN_123 /**< alternative digital input function for
                                                    #IO_PWD_08 */
#define IO_DI_45                    IO_PIN_147 /**< alternative digital input function for
                                                    #IO_PWD_09 */
#define IO_DI_46                    IO_PIN_124 /**< alternative digital input function for
                                                    #IO_PWD_10 */
#define IO_DI_47                    IO_PIN_148 /**< alternative digital input function for
                                                    #IO_PWD_11 */

#define IO_DI_48                    IO_PIN_103 /**< alternative digital input function for
                                                    #IO_ADC_00 */
#define IO_DI_49                    IO_PIN_127 /**< alternative digital input function for
                                                    #IO_ADC_01 */
#define IO_DI_50                    IO_PIN_104 /**< alternative digital input function for
                                                    #IO_ADC_02 */
#define IO_DI_51                    IO_PIN_128 /**< alternative digital input function for
                                                    #IO_ADC_03 */
#define IO_DI_52                    IO_PIN_105 /**< alternative digital input function for
                                                    #IO_ADC_04 */
#define IO_DI_53                    IO_PIN_129 /**< alternative digital input function for
                                                    #IO_ADC_05 */
#define IO_DI_54                    IO_PIN_106 /**< alternative digital input function for
                                                    #IO_ADC_06 */
#define IO_DI_55                    IO_PIN_130 /**< alternative digital input function for
                                                    #IO_ADC_07 */

#define IO_DI_56                    IO_PIN_107 /**< alternative digital input function for
                                                    #IO_ADC_08 */
#define IO_DI_57                    IO_PIN_131 /**< alternative digital input function for
                                                    #IO_ADC_09 */
#define IO_DI_58                    IO_PIN_108 /**< alternative digital input function for
                                                    #IO_ADC_10 */
#define IO_DI_59                    IO_PIN_132 /**< alternative digital input function for
                                                    #IO_ADC_11 */
#define IO_DI_60                    IO_PIN_109 /**< alternative digital input function for
                                                    #IO_ADC_12 */
#define IO_DI_61                    IO_PIN_133 /**< alternative digital input function for
                                                    #IO_ADC_13 */
#define IO_DI_62                    IO_PIN_110 /**< alternative digital input function for
                                                    #IO_ADC_14 */
#define IO_DI_63                    IO_PIN_134 /**< alternative digital input function for
                                                    #IO_ADC_15 */

#define IO_DI_64                    IO_PIN_111 /**< alternative digital input function for
                                                    #IO_ADC_16 */
#define IO_DI_65                    IO_PIN_135 /**< alternative digital input function for
                                                    #IO_ADC_17 */
#define IO_DI_66                    IO_PIN_112 /**< alternative digital input function for
                                                    #IO_ADC_18 */
#define IO_DI_67                    IO_PIN_136 /**< alternative digital input function for
                                                    #IO_ADC_19 */
#define IO_DI_68                    IO_PIN_113 /**< alternative digital input function for
                                                    #IO_ADC_20 */
#define IO_DI_69                    IO_PIN_137 /**< alternative digital input function for
                                                    #IO_ADC_21 */
#define IO_DI_70                    IO_PIN_114 /**< alternative digital input function for
                                                    #IO_ADC_22 */
#define IO_DI_71                    IO_PIN_138 /**< alternative digital input function for
                                                    #IO_ADC_23 */

#define IO_DI_72                    IO_PIN_149 /**< alternative digital input function for
                                                    #IO_DO_00 */
#define IO_DI_73                    IO_PIN_173 /**< alternative digital input function for
                                                    #IO_DO_01 */
#define IO_DI_74                    IO_PIN_152 /**< alternative digital input function for
                                                    #IO_DO_02 */
#define IO_DI_75                    IO_PIN_176 /**< alternative digital input function for
                                                    #IO_DO_03 */
#define IO_DI_76                    IO_PIN_155 /**< alternative digital input function for
                                                    #IO_DO_04 */
#define IO_DI_77                    IO_PIN_179 /**< alternative digital input function for
                                                    #IO_DO_05 */
#define IO_DI_78                    IO_PIN_158 /**< alternative digital input function for
                                                    #IO_DO_06 */
#define IO_DI_79                    IO_PIN_182 /**< alternative digital input function for
                                                    #IO_DO_07 */

#define IO_DI_80                    IO_PIN_251 /**< alternative digital input function for
                                                    #IO_DO_08 */
#define IO_DI_81                    IO_PIN_238 /**< alternative digital input function for
                                                    #IO_DO_09 */
#define IO_DI_82                    IO_PIN_252 /**< alternative digital input function for
                                                    #IO_DO_10 */
#define IO_DI_83                    IO_PIN_239 /**< alternative digital input function for
                                                    #IO_DO_11 */
#define IO_DI_84                    IO_PIN_253 /**< alternative digital input function for
                                                    #IO_DO_12 */
#define IO_DI_85                    IO_PIN_240 /**< alternative digital input function for
                                                    #IO_DO_13 */
#define IO_DI_86                    IO_PIN_254 /**< alternative digital input function for
                                                    #IO_DO_14 */
#define IO_DI_87                    IO_PIN_241 /**< alternative digital input function for
                                                    #IO_DO_15 */

#define IO_DI_88                    IO_PIN_161 /**< alternative digital input function for
                                                    #IO_PVG_00 */
#define IO_DI_89                    IO_PIN_185 /**< alternative digital input function for
                                                    #IO_PVG_01 */
#define IO_DI_90                    IO_PIN_188 /**< alternative digital input function for
                                                    #IO_PVG_02 */
#define IO_DI_91                    IO_PIN_164 /**< alternative digital input function for
                                                    #IO_PVG_03 */
#define IO_DI_92                    IO_PIN_191 /**< alternative digital input function for
                                                    #IO_PVG_04 */
#define IO_DI_93                    IO_PIN_167 /**< alternative digital input function for
                                                    #IO_PVG_05 */
#define IO_DI_94                    IO_PIN_194 /**< alternative digital input function for
                                                    #IO_PVG_06 */
#define IO_DI_95                    IO_PIN_170 /**< alternative digital input function for
                                                    #IO_PVG_07 */

/* Digital outputs */
#define IO_DO_00                    IO_PIN_149 /**< main function high side digital output */
#define IO_DO_01                    IO_PIN_173 /**< main function high side digital output */
#define IO_DO_02                    IO_PIN_152 /**< main function high side digital output */
#define IO_DO_03                    IO_PIN_176 /**< main function high side digital output */
#define IO_DO_04                    IO_PIN_155 /**< main function high side digital output */
#define IO_DO_05                    IO_PIN_179 /**< main function high side digital output */
#define IO_DO_06                    IO_PIN_158 /**< main function high side digital output */
#define IO_DO_07                    IO_PIN_182 /**< main function high side digital output */

#define IO_DO_08                    IO_PIN_251 /**< main function low side digital output */
#define IO_DO_09                    IO_PIN_238 /**< main function low side digital output */
#define IO_DO_10                    IO_PIN_252 /**< main function low side digital output */
#define IO_DO_11                    IO_PIN_239 /**< main function low side digital output */
#define IO_DO_12                    IO_PIN_253 /**< main function low side digital output */
#define IO_DO_13                    IO_PIN_240 /**< main function low side digital output */
#define IO_DO_14                    IO_PIN_254 /**< main function low side digital output */
#define IO_DO_15                    IO_PIN_241 /**< main function low side digital output */

#define IO_DO_16                    IO_PIN_153 /**< alternative digital output function for
                                                    #IO_PWM_00 (shut off group 0) */
#define IO_DO_17                    IO_PIN_177 /**< alternative digital output function for
                                                    #IO_PWM_01 (shut off group 0) */
#define IO_DO_18                    IO_PIN_156 /**< alternative digital output function for
                                                    #IO_PWM_02 (shut off group 0) */
#define IO_DO_19                    IO_PIN_180 /**< alternative digital output function for
                                                    #IO_PWM_03 (shut off group 0) */
#define IO_DO_20                    IO_PIN_159 /**< alternative digital output function for
                                                    #IO_PWM_04 (shut off group 0) */
#define IO_DO_21                    IO_PIN_183 /**< alternative digital output function for
                                                    #IO_PWM_05 (shut off group 0) */
#define IO_DO_22                    IO_PIN_186 /**< alternative digital output function for
                                                    #IO_PWM_06 (shut off group 0) */
#define IO_DO_23                    IO_PIN_162 /**< alternative digital output function for
                                                    #IO_PWM_07 (shut off group 0) */
#define IO_DO_24                    IO_PIN_189 /**< alternative digital output function for
                                                    #IO_PWM_08 (shut off group 0) */
#define IO_DO_25                    IO_PIN_165 /**< alternative digital output function for
                                                    #IO_PWM_09 (shut off group 0) */
#define IO_DO_26                    IO_PIN_192 /**< alternative digital output function for
                                                    #IO_PWM_10 (shut off group 0) */
#define IO_DO_27                    IO_PIN_168 /**< alternative digital output function for
                                                    #IO_PWM_11 (shut off group 0) */
#define IO_DO_28                    IO_PIN_195 /**< alternative digital output function for
                                                    #IO_PWM_12 (shut off group 0) */
#define IO_DO_29                    IO_PIN_171 /**< alternative digital output function for
                                                    #IO_PWM_13 (shut off group 0) */
#define IO_DO_30                    IO_PIN_154 /**< alternative digital output function for
                                                    #IO_PWM_14 (shut off group 1) */
#define IO_DO_31                    IO_PIN_178 /**< alternative digital output function for
                                                    #IO_PWM_15 (shut off group 1) */
#define IO_DO_32                    IO_PIN_157 /**< alternative digital output function for
                                                    #IO_PWM_16 (shut off group 1) */
#define IO_DO_33                    IO_PIN_181 /**< alternative digital output function for
                                                    #IO_PWM_17 (shut off group 1) */
#define IO_DO_34                    IO_PIN_160 /**< alternative digital output function for
                                                    #IO_PWM_18 (shut off group 1) */
#define IO_DO_35                    IO_PIN_184 /**< alternative digital output function for
                                                    #IO_PWM_19 (shut off group 1) */
#define IO_DO_36                    IO_PIN_187 /**< alternative digital output function for
                                                    #IO_PWM_20 (shut off group 1) */
#define IO_DO_37                    IO_PIN_163 /**< alternative digital output function for
                                                    #IO_PWM_21 (shut off group 1) */
#define IO_DO_38                    IO_PIN_190 /**< alternative digital output function for
                                                    #IO_PWM_22 (shut off group 1) */
#define IO_DO_39                    IO_PIN_166 /**< alternative digital output function for
                                                    #IO_PWM_23 (shut off group 1) */
#define IO_DO_40                    IO_PIN_193 /**< alternative digital output function for
                                                    #IO_PWM_24 (shut off group 1) */
#define IO_DO_41                    IO_PIN_169 /**< alternative digital output function for
                                                    #IO_PWM_25 (shut off group 1) */
#define IO_DO_42                    IO_PIN_196 /**< alternative digital output function for
                                                    #IO_PWM_26 (shut off group 1) */
#define IO_DO_43                    IO_PIN_172 /**< alternative digital output function for
                                                    #IO_PWM_27 (shut off group 1) */
#define IO_DO_44                    IO_PIN_101 /**< alternative digital output function for
                                                    #IO_PWM_28 (shut off group 2) */
#define IO_DO_45                    IO_PIN_125 /**< alternative digital output function for
                                                    #IO_PWM_29 (shut off group 2) */
#define IO_DO_46                    IO_PIN_150 /**< alternative digital output function for
                                                    #IO_PWM_30 (shut off group 2) */
#define IO_DO_47                    IO_PIN_174 /**< alternative digital output function for
                                                    #IO_PWM_31 (shut off group 2) */
#define IO_DO_48                    IO_PIN_102 /**< alternative digital output function for
                                                    #IO_PWM_32 (shut off group 2) */
#define IO_DO_49                    IO_PIN_126 /**< alternative digital output function for
                                                    #IO_PWM_33 (shut off group 2) */
#define IO_DO_50                    IO_PIN_151 /**< alternative digital output function for
                                                    #IO_PWM_34 (shut off group 2) */
#define IO_DO_51                    IO_PIN_175 /**< alternative digital output function for
                                                    #IO_PWM_35 (shut off group 2) */

#define IO_DO_52                    IO_PIN_161 /**< alternative digital output function for
                                                    #IO_PVG_00 */
#define IO_DO_53                    IO_PIN_185 /**< alternative digital output function for
                                                    #IO_PVG_01 */
#define IO_DO_54                    IO_PIN_188 /**< alternative digital output function for
                                                    #IO_PVG_02 */
#define IO_DO_55                    IO_PIN_164 /**< alternative digital output function for
                                                    #IO_PVG_03 */
#define IO_DO_56                    IO_PIN_191 /**< alternative digital output function for
                                                    #IO_PVG_04 */
#define IO_DO_57                    IO_PIN_167 /**< alternative digital output function for
                                                    #IO_PVG_05 */
#define IO_DO_58                    IO_PIN_194 /**< alternative digital output function for
                                                    #IO_PVG_06 */
#define IO_DO_59                    IO_PIN_170 /**< alternative digital output function for
                                                    #IO_PVG_07 */
/** @} */

/*
 * D E V I C E S
 */

/**
 * \name Internal devices
 *
 * These channels are used to control internal devices.
 *
 * @{
 */

#define IO_CAN_CHANNEL_0            IO_INT_PIN_CAN_CH0     /**< Internal Pin for CAN channel 0 */
#define IO_CAN_CHANNEL_1            IO_INT_PIN_CAN_CH1     /**< Internal Pin for CAN channel 1 */
#define IO_CAN_CHANNEL_2            IO_INT_PIN_CAN_CH2     /**< Internal Pin for CAN channel 2 */
#define IO_CAN_CHANNEL_3            IO_INT_PIN_CAN_CH3     /**< Internal Pin for CAN channel 3 */
#define IO_CAN_CHANNEL_4            IO_INT_PIN_CAN_CH4     /**< Internal Pin for CAN channel 4 */
#define IO_CAN_CHANNEL_5            IO_INT_PIN_CAN_CH5     /**< Internal Pin for CAN channel 5 */
#define IO_CAN_CHANNEL_6            IO_INT_PIN_CAN_CH6     /**< Internal Pin for CAN channel 6 */

#define IO_INT_SAFETY_SW_0          IO_INT_PIN_SAFETY_SW_0 /**< Internal Pin for safety switch 0 */
#define IO_INT_SAFETY_SW_1          IO_INT_PIN_SAFETY_SW_1 /**< Internal Pin for safety switch 1 */
#define IO_INT_SAFETY_SW_2          IO_INT_PIN_SAFETY_SW_2 /**< Internal Pin for safety switch 2 */
#define IO_INT_POWERSTAGE_ENABLE    IO_INT_PIN_ENDRV_CPU   /**< Internal Pin for enable drive CPU */

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* IO_PIN_H_ */
