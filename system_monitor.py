#!/usr/bin/env python3
# /home/PythonScripts/system_monitor.py

import psutil
import platform
import datetime
import os
import socket
import subprocess

def get_size(bytes, suffix="B"):
    """Convert bytes to human readable format"""
    factor = 1024
    for unit in ["", "K", "M", "G", "T", "P"]:
        if bytes < factor:
            return f"{bytes:.2f}{unit}{suffix}"
        bytes /= factor

def get_network_info():
    """Get network interfaces"""
    interfaces = []
    for iface, addrs in psutil.net_if_addrs().items():
        for addr in addrs:
            if addr.family == socket.AF_INET and not addr.address.startswith('127'):
                interfaces.append({
                    'name': iface,
                    'ip': addr.address,
                    'netmask': addr.netmask
                })
    return interfaces

def get_active_connections():
    """Get active network connections"""
    connections = []
    for conn in psutil.net_connections():
        if conn.status == 'ESTABLISHED' and conn.raddr:
            try:
                connections.append({
                    'local': f"{conn.laddr.ip}:{conn.laddr.port}",
                    'remote': f"{conn.raddr.ip}:{conn.raddr.port}",
                    'pid': conn.pid,
                    'status': conn.status
                })
            except:
                pass
    return connections

def get_kali_tools():
    """Check if popular Kali tools are installed"""
    tools = [
        'nmap', 'wireshark', 'metasploit-framework', 'burpsuite',
        'john', 'hydra', 'sqlmap', 'aircrack-ng'
    ]
    
    installed = []
    for tool in tools:
        result = subprocess.run(['which', tool], capture_output=True, text=True)
        if result.returncode == 0:
            installed.append(tool)
    
    return installed

def print_section(title):
    """Print formatted section"""
    print(f"\n{'='*60}")
    print(f" {title}")
    print(f"{'='*60}")

def main():
    print(f"""
    ╔══════════════════════════════╗
    ║     KALI SYSTEM MONITOR      ║
    ║   {datetime.datetime.now().strftime('%Y-%m-%d %H:%M')}   ║
    ╚══════════════════════════════╝
    """)
    
    # System Info
    print_section("SYSTEM INFORMATION")
    uname = platform.uname()
    print(f"System: {uname.system}")
    print(f"Hostname: {socket.gethostname()}")
    print(f"Kernel: {uname.release}")
    print(f"Architecture: {uname.machine}")
    
    # CPU Info
    print_section("CPU INFORMATION")
    print(f"Physical cores: {psutil.cpu_count(logical=False)}")
    print(f"Total cores: {psutil.cpu_count(logical=True)}")
    print(f"CPU Usage: {psutil.cpu_percent(interval=1)}%")
    
    # Memory Info
    print_section("MEMORY INFORMATION")
    svmem = psutil.virtual_memory()
    print(f"Total: {get_size(svmem.total)}")
    print(f"Available: {get_size(svmem.available)}")
    print(f"Used: {get_size(svmem.used)} ({svmem.percent}%)")
    
    # Disk Info
    print_section("DISK INFORMATION")
    partitions = psutil.disk_partitions()
    for partition in partitions:
        if partition.fstype:
            try:
                usage = psutil.disk_usage(partition.mountpoint)
                print(f"Device: {partition.device}")
                print(f"Mount: {partition.mountpoint}")
                print(f"Type: {partition.fstype}")
                print(f"Size: {get_size(usage.total)}")
                print(f"Used: {get_size(usage.used)} ({usage.percent}%)")
                print("-" * 40)
            except PermissionError:
                continue
    
    # Network Info
    print_section("NETWORK INFORMATION")
    interfaces = get_network_info()
    for iface in interfaces:
        print(f"Interface: {iface['name']}")
        print(f"IP: {iface['ip']}")
        print(f"Netmask: {iface['netmask']}")
        print("-" * 40)
    
    # Active Connections
    print_section("ACTIVE CONNECTIONS")
    connections = get_active_connections()
    if connections:
        for conn in connections[:10]:  # Show first 10
            print(f"Local: {conn['local']:<22} | Remote: {conn['remote']:<22} | PID: {conn['pid']}")
    else:
        print("No active connections")
    
    # Kali Tools
    print_section("INSTALLED KALI TOOLS")
    tools = get_kali_tools()
    if tools:
        print(", ".join(tools))
    else:
        print("No common Kali tools found in PATH")
    
    # Battery
    if hasattr(psutil, "sensors_battery"):
        battery = psutil.sensors_battery()
        if battery:
            print_section("BATTERY")
            print(f"Percentage: {battery.percent}%")
            print(f"Plugged in: {battery.power_plugged}")
    
    print(f"\n{'='*60}")

if __name__ == "__main__":
    try:
        main()
    except KeyboardInterrupt:
        print("\n[!] Interrupted")
    except Exception as e:
        print(f"[-] Error: {e}")