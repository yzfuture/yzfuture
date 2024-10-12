import ctypes
import platform
import enum
# 定义枚举类型
class eCardFormatType(enum.Enum):
    eTwoIDType = ord(' ')
    eTwoGATType = ord('J')
    eOldForeignerType = ord('I')
    eNewForeignerType = ord('Y')

# 定义C++结构体对应的ctypes结构体
class TwoIdInfoStruct(ctypes.Structure):
    _fields_ = [
        ("arrName", ctypes.c_char * 30),  # UNICODE姓名
        ("arrSex", ctypes.c_char * 2),    # UNICODE性别
        ("arrNation", ctypes.c_char * 4), # UNICODE民族
        ("arrBirthday", ctypes.c_char * 16), # UNICODE出生日期 YYYYMMDD
        ("arrAddress", ctypes.c_char * 70), # UNICODE住址
        ("arrNo", ctypes.c_char * 36),    # UNICODE身份证号码
        ("arrSignedDepartment", ctypes.c_char * 30), # UNICODE签发机关
        ("arrValidityPeriodBegin", ctypes.c_char * 16), # UNICODE有效期起始日期 YYYYMMDD
        ("arrValidityPeriodEnd", ctypes.c_char * 16), # UNICODE有效期截止日期 YYYYMMDD
        ("arrOtherNO", ctypes.c_char * 18), # 通行证类号码
        ("arrSignNum", ctypes.c_char * 4),  # 签发次数
        ("arrRemark1", ctypes.c_char * 6),  # 预留区
        ("arrType", ctypes.c_char * 2),    # 证件类型标识
        ("arrRemark2", ctypes.c_char * 6),  # 预留区
        ("arrPhoto", ctypes.c_ubyte * 1024), # 照片信息
        ("arrFingerprint", ctypes.c_ubyte * 1024), # 指纹信息
    ]

class ForeignerInfoOld(ctypes.Structure):
    _fields_ = [
        ("arrEnName", ctypes.c_char * 120), # 英文名
        ("arrSex", ctypes.c_char * 2),      # UNICODE性别
        ("arrNo", ctypes.c_char * 30),      # 15个字符的居留证号码 UNICODE
        ("arrCountry", ctypes.c_char * 6),  # 国籍 UNICODE GB/T2659-2000
        ("arrName", ctypes.c_char * 30),    # 中文姓名 UNICODE
        ("arrValidityPeriodBegin", ctypes.c_char * 16), # 签发日期 UNICODE YYYYMMDD
        ("arrValidityPeriodEnd", ctypes.c_char * 16),   # 终止日期 UNICODE YYYYMMDD
        ("arrBirthday", ctypes.c_char * 16),            # 出生日期 UNICODE YYYYMMDD
        ("arrVersion", ctypes.c_char * 4),              # 版本号
        ("arrSignedDepartment", ctypes.c_char * 8),     # 签发机关代码 UNICODE
        ("arrType", ctypes.c_char * 2),                 # 证件类型标识
        ("arrRemark2", ctypes.c_char * 6),              # 预留区
        ("arrPhoto", ctypes.c_ubyte * 1024),            # 照片信息
        ("arrFingerprint", ctypes.c_ubyte * 1024),      # 指纹信息
    ]

class ForeignerInfoNew(ctypes.Structure):
    _fields_ = [
        ("arrName", ctypes.c_char * 30),                # UNICODE姓名
        ("arrSex", ctypes.c_char * 2),                  # UNICODE性别
        ("arrNation", ctypes.c_char * 4),               # UNICODE民族
        ("arrBirthday", ctypes.c_char * 16),            # UNICODE出生日期 YYYYMMDD
        ("arrEnName", ctypes.c_char * 70),              # 外文姓名 UNICODE
        ("arrNo", ctypes.c_char * 36),                  # UNICODE身份证号码
        ("arrSignedDepartment", ctypes.c_char * 30),    # UNICODE签发机关
        ("arrValidityPeriodBegin", ctypes.c_char * 16), # UNICODE有效期起始日期 YYYYMMDD
        ("arrValidityPeriodEnd", ctypes.c_char * 16),   # UNICODE有效期截止日期 YYYYMMDD
        ("arrOtherNO", ctypes.c_char * 18),             # 通行证类号码
        ("arrSignNum", ctypes.c_char * 4),              # 签发次数
        ("arrCountry", ctypes.c_char * 6),              # 国籍 UNICODE GB/T2659-2000
        ("arrType", ctypes.c_char * 2),                 # 证件类型标识
        ("arrRemark2", ctypes.c_char * 6),              # 预留区
        ("arrPhoto", ctypes.c_ubyte * 1024),            # 照片信息
        ("arrFingerprint", ctypes.c_ubyte * 1024),      # 指纹信息
    ]

class CardInfoStruct(ctypes.Union):
    _fields_ = [
        ("twoId", TwoIdInfoStruct),
        ("foreigner", ForeignerInfoOld),
        ("newForeigner", ForeignerInfoNew),
    ]

class CardInfoStructWrapper(ctypes.Structure):
    _fields_ = [
        ("etype", ctypes.c_char),
        ("info", CardInfoStruct),
    ]


# 假设libreadcard.so是已经编译好的库文件，加载对应SDK文件夹里面的so
libreadcard = ctypes.CDLL('/root/pythonDemo/libreadCardInfo_x86_linux.so')


# 设置设备类型
def set_device_type(device_type):
    result = libreadcard.setDeviceType(device_type)
    return result


# 登录解码服务器
def login_card_server(server_ip, port):
    error_code = ctypes.c_uint()
    result = libreadcard.loginCardServerEx(server_ip.encode('utf-8'), port, ctypes.byref(error_code))
    return result


# 打开读卡器设备
def card_open_device(device_type):
    error_code = ctypes.c_uint()
    handle = libreadcard.cardOpenDevice(2, ctypes.byref(error_code), device_type)
    if handle > 0:
        print("读卡器设备打开成功")
        return handle
    else:
        print("读卡器设备打开失败，错误码: {error_code.value}")
        return None


# 设置卡片类型
def set_card_type(handle, card_type):
    result = libreadcard.setCardType(handle, card_type)
    return result


# 寻找卡片
def card_find_card(handle):
    found = ctypes.c_bool()
    result = libreadcard.cardFindCard(handle, ctypes.byref(found))
    return result, found


# 读取卡片信息
def card_read_two_card_ex(handle):
    card_info = CardInfoStructWrapper()
    result = libreadcard.cardReadTwoCardEx(handle, None, None, ctypes.byref(card_info))
    if result:
        print("读取卡片信息成功")
        libreadcard.convertCardInfoUncodeToUtf8Ex(ctypes.byref(card_info))
        if card_info.etype == eCardFormatType.eOldForeignerType.value:
            name = card_info.info.foreigner.arrName.decode('utf-8').rstrip('\x00')
            print("旧版外国人居住证姓名:", name)
        elif card_info.etype == eCardFormatType.eNewForeignerType.value:
            name = card_info.info.newForeigner.arrName.decode('utf-8').rstrip('\x00')
            print("新版外国人居住证姓名:", name)
        else:
            name = card_info.info.twoId.arrName.decode('utf-8').rstrip('\x00')
            print("身份证姓名:", name)
    else:
        print("读取卡片信息失败")
    libreadcard.cardBeep(handle)






# 关闭读卡器设备
def card_close_device(handle):
    libreadcard.cardCloseDevice(handle)


# 登出解码服务器
def logout_card_server():
    libreadcard.logoutCardServer()


# 反初始化操作
def card_read_uninit():
    libreadcard.cardReadUninit()



# 主函数
def main():
    # 初始化读卡器
    libreadcard.cardReadInit()

    # 设置设备类型
    set_device_type(0)  # 0代表鱼住标准读卡器

    # 登录解码服务器
    if login_card_server('id.yzfuture.cn', 443):
        print("登录成功")
    else:
        print("登录失败")

    # 打开读卡器设备
    handle = card_open_device(0)
    if handle:
        # 设置卡片类型为BCardType
        if set_card_type(handle, 1):
            print("卡片类型设置成功")

            # 寻找卡片
            found, found_card = card_find_card(handle)
            if found:
                print("找到卡片")
                # 读取卡片信息
                card_read_two_card_ex(handle)

            else:
                print("未找到卡片")
        else:
            print("设置卡片类型失败")
    else:
        print("读卡器设备打开失败")

    # 关闭读卡器设备
    card_close_device(handle)

    # 登出解码服务器
    logout_card_server()

    # 反初始化操作
    card_read_uninit()


if __name__ == "__main__":
    main()
