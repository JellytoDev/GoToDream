using System;
using System.Net;
using System.Runtime.InteropServices;

public sealed class ARP
{
    [StructLayout(LayoutKind.Explicit)]
    private struct IPAddr
    {
        [FieldOffset(0)]
        public byte s_b1;
        [FieldOffset(1)]
        public byte s_b2;
        [FieldOffset(2)]
        public byte s_b3;
        [FieldOffset(3)]
        public byte s_b4;
        [FieldOffset(0)]
        public ushort s_w1;
        [FieldOffset(2)]
        public ushort s_w2;
        [FieldOffset(0)]
        public uint s_addr;

        // IP 주소 문자열 값 할당
        public void Set(string address)
        {
            IPAddress ip = IPAddress.Parse(address);
            Set(ip);
        }

        // System.Net.IPAddress 객체 값 할당
        public void Set(IPAddress ip)
        {
            byte[] addrBytes = ip.GetAddressBytes();
            s_b1 = addrBytes[0];
            s_b2 = addrBytes[1];
            s_b3 = addrBytes[2];
            s_b4 = addrBytes[3];
        }

        public static IPAddr Empty = new IPAddr();
    }

    // SendARP API 선언
    [DllImport("iphlpapi.dll", EntryPoint = "SendARP")]
    private static extern uint _SendARP(IPAddr DestIP, IPAddr SrcIP,
        [In, Out] byte[] pMacAddr, ref int PhyAddrLen);

    // ARP을 통해 MAC 주소를 가져온다.
    public static byte[] GetMacAddress(string destIP)
    {
        IPAddr destIPAddr = new IPAddr();
        IPAddr srcIPAddr = IPAddr.Empty;
        uint result = 0;
        byte[] mac = new byte[6];
        int len = mac.Length;

        destIPAddr.Set(destIP);
        result = _SendARP(destIPAddr, srcIPAddr, mac, ref len);
        if (result == 0)
        {
            return mac;
        }
        return null;   // 오류코드를 담은 Exception을 생성하는 것도 좋은 생각이지만... 귀차니즘 땀시... -_-;;
    }

}

struct arpDevice
{
    public string ip;
    public string state;
    public byte[] mac;
};

class TestClass
{

    static void Main(string[] args)
    {
       
        string targetIP = "52.114.133.60";

        var device1= MacOutput(targetIP);

        Console.Write("target ip : {0} {1}\n", device1.ip, device1.state);

        if (device1.mac != null)
        {
            Console.Write("MAC = ");
            for (int i = 0; i < device1.mac.Length; i++)
            {
                Console.Write("{0:x2}", device1.mac[i]);
                if (i != 5)
                    Console.Write('-');
            }
            Console.WriteLine();
        }
        else
        {
            Console.WriteLine("ARP으로 MAC 주소를 알아낼 수 없습니다.");
        }

    }
    //Tuple<string, string, byte[]>
    static arpDevice MacOutput(string targetIP)
    {
        arpDevice target_device;
        byte[] mac;

        mac = ARP.GetMacAddress(targetIP);

        if (mac != null)
        {
            target_device.ip = targetIP;
            target_device.mac = mac;
            target_device.state = "success";
        }
        else
        {
            target_device.ip = targetIP;
            target_device.mac = mac;
            target_device.state = "fail";
        }
        return target_device;
    }


}

