#!/usr/bin/env python3
# /home/PythonScripts/scan.py

import subprocess
import socket
import sys
from datetime import datetime
import ipaddress

def print_banner():
    print("""
    ╔══════════════════════════════╗
    ║     NETWORK SCANNER v1.0     ║
    ╚══════════════════════════════╝
    """)

def get_local_ip():
    """Получаем локальный IP и сеть"""
    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    try:
        s.connect(('8.8.8.8', 1))
        local_ip = s.getsockname()[0]
        return local_ip
    except Exception:
        return "127.0.0.1"
    finally:
        s.close()

def scan_network(network):
    """Быстрый ARP-скан сети"""
    print(f"[*] Scanning network: {network}")
    print("[*] Start time: " + str(datetime.now()))
    print("-" * 50)
    
    
    try:
        result = subprocess.run(['sudo', 'arp-scan', '--localnet'], 
                              capture_output=True, text=True, timeout=30)
        
        devices = []
        for line in result.stdout.split('\n'):
            if '\t' in line:  
                parts = line.split('\t')
                if len(parts) >= 2:
                    ip = parts[0]
                    mac = parts[1]
                    vendor = parts[2] if len(parts) > 2 else "Unknown"
                    devices.append({'ip': ip, 'mac': mac, 'vendor': vendor})
        
        return devices
    except:
        
        return ping_sweep(network)

def ping_sweep(network):
    """Ping sweep если arp-scan не работает"""
    devices = []
    network = ipaddress.ip_network(network, strict=False)
    
    print("[*] Using ping sweep (slower)...")
    for ip in network.hosts():
        ip_str = str(ip)
        result = subprocess.run(['ping', '-c', '1', '-W', '1', ip_str], 
                              stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
        if result.returncode == 0:
            try:
                hostname = socket.gethostbyaddr(ip_str)[0]
            except:
                hostname = "Unknown"
            devices.append({'ip': ip_str, 'hostname': hostname})
            print(f"[+] {ip_str} - {hostname}")
    
    return devices

def port_scan(ip, ports=[22, 80, 443, 8080, 3306, 5432]):
    """Быстрый scan популярных портов"""
    print(f"\n[*] Scanning ports on {ip}")
    open_ports = []
    
    for port in ports:
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        sock.settimeout(0.5)
        result = sock.connect_ex((ip, port))
        if result == 0:
            service = socket.getservbyport(port, 'tcp') if port <= 65535 else "unknown"
            open_ports.append({'port': port, 'service': service})
            print(f"[+] Port {port}/tcp - {service}")
        sock.close()
    
    return open_ports

def main():
    print_banner()
    
    
    local_ip = get_local_ip()
    if local_ip == "127.0.0.1":
        network = input("Enter network (e.g., 192.168.1.0/24): ")
    else:
        ip_parts = local_ip.split('.')
        network = f"{ip_parts[0]}.{ip_parts[1]}.{ip_parts[2]}.0/24"
        print(f"[+] Local IP: {local_ip}")
        print(f"[+] Network: {network}")
    
    
    devices = scan_network(network)
    
    if devices:
        print(f"\n[+] Found {len(devices)} devices:")
        print("-" * 70)
        for device in devices:
            print(f"IP: {device.get('ip', 'N/A'):<15} | "
                  f"MAC: {device.get('mac', 'N/A'):<17} | "
                  f"Vendor: {device.get('vendor', device.get('hostname', 'N/A'))}")
            
            
            answer = input(f"\nScan ports on {device.get('ip', '')}? (y/n): ")
            if answer.lower() == 'y':
                port_scan(device.get('ip', ''))
    else:
        print("[-] No devices found")
    
    print("-" * 50)
    print("[*] Scan completed at: " + str(datetime.now()))

if __name__ == "__main__":
    try:
        main()
    except KeyboardInterrupt:
        print("\n[!] Scan interrupted by user")
        sys.exit(0)
    except Exception as e:
        print(f"[-] Error: {e}")
        sys.exit(1)