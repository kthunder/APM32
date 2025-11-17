import usb.core
import usb.util
import time

dev = usb.core.find(idVendor=0x0D28, idProduct=0x0204)
if dev is None:
    raise ValueError("设备未找到")

dev.set_configuration()
cfg = dev.get_active_configuration()

# 查找第一个有端点的0xFF接口
intf = None
for i in cfg:
    if i.bInterfaceClass == 0xFF and i.bNumEndpoints > 0:
        intf = i
        break

usb.util.claim_interface(dev, intf.bInterfaceNumber)

ep_out = usb.util.find_descriptor(
    intf,
    custom_match=lambda e: e.bEndpointAddress & 0x80 == 0 and e.bmAttributes == 0x02
)
ep_in = usb.util.find_descriptor(
    intf,
    custom_match=lambda e: e.bEndpointAddress & 0x80 != 0 and e.bmAttributes == 0x02
)

print(ep_out)
print(ep_in)

def speed_test():
    bytelen = 64 * 10000
    data = b'\xAA' * bytelen
    start = time.time()
    ep_out.write(data)
    duration = time.time() - start
    print(f"速度: {bytelen / duration / 1024:.2f} KB/s")

def dap_info(id):
    packet = bytearray([0x00, id] + [0] * 62)  # 填充到64字节
    ep_out.write(packet)
    return ep_in.read(64, timeout=1000)

if __name__ == "__main__":
    # 获取厂商信息 (ID=0x01)
    info = dap_info(0x01)
    print("厂商:", len(info))
    print("厂商:", hex(info[0]))
    print("厂商:", hex(info[1]))
    # ep_out.write(bytearray([0x7e, 1, 0x80, 1]))
    # try:
    #     data = ep_in.read(64, timeout=1000)
    #     print("接收数据:", [hex(x) for x in data])
    # except usb.core.USBError as e:
    #     print(f"USB错误: {e}")
    pass