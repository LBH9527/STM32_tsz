HID 鼠标
Bus抓取的数据：

Bus Hound 6.01 capture on Windows Vista Service Pack 1 (x86). Complements of www.perisoft.net

txt

  Device - Device ID (followed by the endpoint for USB devices)
            (57) USB ????
            (58) HID-compliant mouse
            (60) STM Device in DFU Mode
  Length - Total transfer length
  Phase  - Phase Type
            CTL   USB control transfer       
            IN    Data in transfer           
  Data   - Hex dump of the data transferred
  Descr  - Description of the phase
  Cmd... - Position in the captured data
  Time   - Time the phase occurred in hour:minute:second.millisec form


Device  Length    Phase  Data                                                Description       Cmd.Phase.Ofs(rep)  Time        
------  --------  -----  --------------------------------------------------  ----------------  ------------------  ------------
  57.0            CTL    80 06 00 01  00 00 12 00                            GET DESCRIPTOR           1.1.0        09:44:08.359
						获取设备描述符
  57.0        18  IN     12 01 00 02  00 00 00 40  83 04 10 57  00 02 01 02  .......@...W....         1.2.0        09:44:08.359  
                         03 01                                               ..                       1.2.16     
						 
  57.0            CTL    80 06 00 02  00 00 09 00                            GET DESCRIPTOR           2.1.0        09:44:08.359  
						获取配置描述符
  57.0         9  IN     09 02 22 00  01 01 00 e0  32                        ..".....2                2.2.0        09:44:08.359  
						“22” 配置描述符总长度 
  57.0            CTL    80 06 00 02  00 00 22 00                            GET DESCRIPTOR           3.1.0        09:44:08.359  
						获取全部配置描述符，配置描述符+ 接口描述符 + HID描述符 + 端点描述符
  57.0        34  IN     09 02 22 00  01 01 00 e0  32 09 04 00  00 01 03 01  ..".....2.......         3.2.0        09:44:08.359  
                         02 00 09 21  11 01 00 01  22 4a 00 07  05 81 03 04  ...!...."J......         3.2.16                     
                         00 0a                                               ..                       3.2.32                     
  57.0            CTL    00 09 01 00  00 00 00 00                            SET CONFIG               4.1.0        09:44:08.359  
  57.0            CTL    21 0a 00 00  00 00 00 00                            SET IDLE                 5.1.0        09:44:08.359  
  57.0            CTL    81 06 00 22  00 00 8a 00                            GET DESCRIPTOR           6.1.0        09:44:08.359  
  57.0        74  IN     05 01 09 02  a1 01 09 01  a1 00 05 09  19 01 29 03  ..............).         6.2.0        09:44:08.359  
                         15 00 25 01  95 03 75 01  81 02 95 01  75 05 81 01  ..%...u.....u...         6.2.16                     
                         05 01 09 30  09 31 09 38  15 81 25 7f  75 08 95 03  ...0.1.8..%.u...         6.2.32                     
                         81 06 c0 09  3c 05 ff 09  01 15 00 25  01 75 01 95  ....<......%.u..         6.2.48                     
