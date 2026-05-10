#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <limits>
#include <string>
#include <vector>
#include <sstream>
#include <sys/stat.h>
#include <thread>
#include <chrono>

using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::vector;

#define RED     "\033[1;31m"
#define GREEN   "\033[1;32m"
#define YELLOW  "\033[1;33m"
#define BLUE    "\033[1;34m"
#define CYAN    "\033[1;36m"
#define MAGENTA "\033[1;35m"
#define RESET   "\033[0m"

void showStatus(string text, string color);
void statusInfo(string text);
void statusWork(string text);
void statusError(string text);
void statusOk(string text);
void pauseScreen();
void showMenu();
int getChoice();
void webTest();
void exploitation();
void sniffingSpoofing();
void information();
void kaliTools();
void installOsintTools();
void usernameSearch();
void domainInfo();
void osintMenu();
void connectEsp();
void flipperZero();
void deviceMenu();
void programmInfo();
void donate();
void slowPrint(const std::string& text, int delayMs = 40);

class PythonScripts {
private:
    string create_temp_file(const string& content) {
        string filename = "/tmp/kali_helper_" + std::to_string(getpid()) + ".py";
        std::ofstream file(filename);
        if (!file.is_open()) {
            std::cerr << RED << "[-] Failed to create temporary file!" << RESET << std::endl;
            return "";
        }
        file << content;
        file.close();
        chmod(filename.c_str(), 0755);
        return filename;
    }
    
    void run_python_script(const string& script_content, const string& args = "") {
        string filename = create_temp_file(script_content);
        if (filename.empty()) return;
        
        string command = "python3 " + filename + " " + args;
        cout << CYAN << "[*] Launch Python Scripts..." << RESET << std::endl;
        
        int result = system(command.c_str());
        
        remove(filename.c_str());
        
        if (result != 0) {
            cout << RED << "[-] Python script execution error (Code: " << result << ")" << RESET << endl;
        }
    }

public:
    void network_scan() {
        cout << CYAN << "[*] Launch Network scanner..." << RESET << endl;
        
        const char* script = 
"#!/usr/bin/env python3\n"
"import subprocess\n"
"import socket\n"
"import sys\n"
"import ipaddress\n"
"from datetime import datetime\n"
"\n"
"def get_local_ip():\n"
"    try:\n"
"        s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)\n"
"        s.connect(('8.8.8.8', 1))\n"
"        ip = s.getsockname()[0]\n"
"        s.close()\n"
"        return ip\n"
"    except:\n"
"        return None\n"
"\n"
"def scan_ports(ip):\n"
"    print(f\"\\n[*] Scanning Ports {ip}\")\n"
"    common_ports = [21, 22, 23, 25, 53, 80, 110, 111, 135, 139, 143, 443, 445, 993, 995, 1723, 3306, 3389, 5900, 8080]\n"
"    open_ports = []\n"
"    \n"
"    for port in common_ports:\n"
"        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)\n"
"        sock.settimeout(0.5)\n"
"        result = sock.connect_ex((ip, port))\n"
"        if result == 0:\n"
"            try:\n"
"                service = socket.getservbyport(port)\n"
"            except:\n"
"                service = \"unknown\"\n"
"            open_ports.append((port, service))\n"
"            print(f\"  [+] Port {port} open ({service})\")\n"
"        sock.close()\n"
"    return open_ports\n"
"\n"
"def main():\n"
"    print(\"\\n╔══════════════════════════════╗\")\n"
"    print(\"║     NETWORK SCANNER v2.0     ║\")\n"
"    print(\"╚══════════════════════════════╝\\n\")\n"
"    \n"
"    local_ip = get_local_ip()\n"
"    if local_ip:\n"
"        print(f\"[+] Local IP: {local_ip}\")\n"
"        ip_parts = local_ip.split('.')\n"
"        default_network = f\"{ip_parts[0]}.{ip_parts[1]}.{ip_parts[2]}.0/24\"\n""        network = input(f\"Enter a net [Enter for {default_network}]: \") or default_network\n"
"    else:\n"
"        network = input(\"Enter a net (example, 192.168.1.0/24): \")\n"
"    \n"
"    print(f\"\\n[*] Scanning {network}...\")\n"
"    print(f\"[*] Start time: {datetime.now()}\\n\")\n"
"    \n"
"    try:\n"
"        # Checking availability arp-scan\n"
"        try:\n"
"            subprocess.run(['which', 'arp-scan'], check=True, capture_output=True)\n"
"            use_arp = True\n"
"        except:\n"
"            use_arp = False\n"
"            print(\"[!] arp-scan not found, starting ping Scanning\")\n"
"        \n"
"        devices = []\n"
"        \n"
"        if use_arp:\n"
"            # ARP Scanning\n"
"            result = subprocess.run(['sudo', 'arp-scan', '--localnet'], \n"
"                                  capture_output=True, text=True, timeout=30)\n"
"            \n"
"            for line in result.stdout.split('\\n'):\n"
"                if '\\t' in line:\n"
"                    parts = line.split('\\t')\n"
"                    if len(parts) >= 2:\n"
"                        ip = parts[0]\n"
"                        mac = parts[1]\n"
"                        vendor = parts[2] if len(parts) > 2 else \"Unknown\"\n"
"                        devices.append((ip, mac, vendor))\n"
"        else:\n"
"            network_obj = ipaddress.ip_network(network, strict=False)\n"
"            print(\"[*] Running ping Scan...\")\n"
"            for ip in network_obj.hosts():\n"
"                ip_str = str(ip)\n"
"                result = subprocess.run(['ping', '-c', '1', '-W', '1', ip_str], \n"
"                                      stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)\n"
"                if result.returncode == 0:\n"
"                    devices.append((ip_str, \"N/A\", \"Unknown\"))\n"
"                    print(f\"  [+] {ip_str} - active\")\n"
"        \n"
"        if devices:\n"
"            print(f\"\\n[+] Found devices: {len(devices)}\")\n"
"            print(\"-\" * 70)\n"
"            for ip, mac, vendor in devices:\n"
"                print(f\"IP: {ip:<15} | MAC: {mac:<17} | Vendor: {vendor}\")\n"
"                \n"
"                # Scanning ports each device\n"
"                answer = input(f\"\\n Scan ports on {ip}? (y/n): \")\n"
"                if answer.lower() == 'y':\n"
"                    scan_ports(ip)\n"
"        else:\n"
"            print(\"[-] no devices found\")\n"
"            \n"
"    except subprocess.TimeoutExpired:\n"
"        print(\"[-] scan timeout\")\n"
"    except Exception as e:\n"
"        print(f\"[-] Error: {e}\")\n"
"    \n"
"    print(f\"\\n[*] Scan end: {datetime.now()}\")\n"
"\n"
"if name == \"main\":\n"
"    try:\n"
"        main()\n"
"    except KeyboardInterrupt:\n"
"        print(\"\\n[!] scanning interrupted\")\n"
"        sys.exit(0)\n"
"    except Exception as e:\n"
"        print(f\"[-] Error: {e}\")\n"
"        sys.exit(1)\n";
        
        run_python_script(script);
        pauseScreen();
    }
    
    void system_monitor() {
        cout << CYAN << "[*] Launching System Monitor..." << RESET << endl;
        
        const char* script = 
"#!/usr/bin/env python3\n"
"import psutil\n"
"import platform\n"
"import datetime\n"
"import socket\n"
"import os\n"
"\n"
"def get_size(bytes):\n"
"    for unit in ['', 'K', 'M', 'G', 'T', 'P']:\n"
"        if bytes < 1024:\n"
"            return f\"{bytes:.2f}{unit}B\"\n"
"        bytes /= 1024\n"
"\n"
"def print_section(title):\n"
"    print(f\"\\n{'='*60}\")\n"
"    print(f\" {title}\")\n"
"    print(f\"{'='*60}\")\n"
"\n"
"def main():\n"
"    print(f\"\"\"\n"
"    ╔══════════════════════════════╗\n"
"    ║     SYSTEM MONITOR v2.0      ║\n"
"    ║   {datetime.datetime.now().strftime('%Y-%m-%d %H:%M')}   ║\n""    ╚══════════════════════════════╝\n"
"    \"\"\")\n"
"    \n"
"    print_section(\"SYSTEM INFORMATION\")\n"
"    uname = platform.uname()\n"
"    print(f\"System: {uname.system}\")\n"
"    print(f\"Host name: {socket.gethostname()}\")\n"
"    print(f\"Core: {uname.release}\")\n"
"    print(f\"Architecture: {uname.machine}\")\n"
"    \n"
"    print_section(\"CPU\")\n"
"    print(f\"Physical cores: {psutil.cpu_count(logical=False)}\")\n"
"    print(f\"logical cores: {psutil.cpu_count(logical=True)}\")\n"
"    print(f\"CPU Load: {psutil.cpu_percent(interval=1)}%\")\n"
"    \n"
"    print_section(\"RAM\")\n"
"    mem = psutil.virtual_memory()\n"
"    print(f\"Total: {get_size(mem.total)}\")\n"
"    print(f\"Available: {get_size(mem.available)}\")\n"
"    print(f\"Used: {get_size(mem.used)} ({mem.percent}%)\")\n"
"    \n"
"    print_section(\"DISKS\")\n"
"    for part in psutil.disk_partitions():\n"
"        if part.fstype:\n"
"            try:\n"
"                usage = psutil.disk_usage(part.mountpoint)\n"
"                print(f\"Device: {part.device}\")\n"
"                print(f\"Mount Point: {part.mountpoint}\")\n"
"                print(f\"File System: {part.fstype}\")\n"
"                print(f\"Total: {get_size(usage.total)}\")\n"
"                print(f\"Used: {get_size(usage.used)} ({usage.percent}%)\")\n"
"                print(\"-\" * 40)\n"
"            except PermissionError:\n"
"                continue\n"
"    \n"
"    print_section(\"Network Interfaces\")\n"
"    for iface, addrs in psutil.net_if_addrs().items():\n"
"        for addr in addrs:\n"
"            if addr.family == socket.AF_INET and not addr.address.startswith('127'):\n"
"                print(f\"Interface: {iface}\")\n"
"                print(f\"IP: {addr.address}\")\n"
"                print(f\"Netmask: {addr.netmask}\")\n"
"                print(\"-\" * 40)\n"
"    \n"
"    print_section(\"Top 5 processes by CPU\")\n"
"    processes = []\n"
"    for proc in psutil.process_iter(['pid', 'name', 'cpu_percent']):\n"
"        try:\n"
"            processes.append(proc.info)\n"
"        except:\n"
"            pass\n"
"    \n"
"    for proc in sorted(processes, key=lambda x: x['cpu_percent'] or 0, reverse=True)[:5]:\n"
"        try:\n"
"            print(f\"PID: {proc['pid']:<6} | CPU: {proc['cpu_percent']:.1f}% | {proc['name']}\")\n"
"        except:\n"
"            pass\n"
"\n"
"if name == \"main\":\n"
"    try:\n"
"        main()\n"
"    except Exception as e:\n"
"        print(f\"Error: {e}\")\n";
        
        run_python_script(script);
        pauseScreen();
    }
    
    void service_manager() {
        cout << CYAN << "[*] Launching the Service Manager..." << RESET << endl;
        
        const char* script = 
"#!/usr/bin/env python3\n"
"import subprocess\n"
"import os\n"
"import sys\n"
"\n"
"class ServiceManager:\n"
"    def init(self):\n"
"        self.services = self.get_services()\n"
"    \n"
"    def get_services(self):\n"
"        services = []\n"
"        try:\n"
"            result = subprocess.run(['systemctl', 'list-units', '--type=service', '--all', '--no-pager'],\n"
"                                  capture_output=True, text=True)\n"
"            \n"
"            for line in result.stdout.split('\\n')[1:]:\n"
"                parts = line.split()\n"
"                if len(parts) >= 4 and '.' in parts[0]:\n"
"                    services.append({\n"
"                        'name': parts[0].replace('.service', ''),\n"
"                        'active': parts[2],\n"
"                        'sub': parts[3]\n"
"                    })\n"
"        except Exception as e:\n"
"            print(f\"Error Receiving Services: {e}\")\n""        return services\n"
"    \n"
"    def show_menu(self):\n"
"        while True:\n"
"            os.system('clear')\n"
"            print(\"\"\"\n"
"    ╔══════════════════════════════╗\n"
"    ║     SERVICE MANAGER v2.0     ║\n"
"    ╚══════════════════════════════╝\n"
"            \"\"\")\n"
"            \n"
"            print(\"[1] Show All Services\")\n"
"            print(\"[2] Show Active Services\")\n"
"            print(\"[3] Show Stopped Services\")\n"
"            print(\"[4] Service management\")\n"
"            print(\"[5] Check Service Logs\")\n"
"            print(\"[0] Back\")\n"
"            \n"
"            choice = input(\"\\nChoice Option: \").strip()\n"
"            \n"
"            if choice == '1':\n"
"                self.show_services()\n"
"            elif choice == '2':\n"
"                self.show_services(filter_active=True)\n"
"            elif choice == '3':\n"
"                self.show_services(filter_inactive=True)\n"
"            elif choice == '4':\n"
"                self.manage_service()\n"
"            elif choice == '5':\n"
"                self.show_logs()\n"
"            elif choice == '0':\n"
"                break\n"
"            \n"
"            input(\"\\nPress Enter to Continue...\")\n"
"    \n"
"    def show_services(self, filter_active=False, filter_inactive=False):\n"
"        print(\"\\n\" + \"=\"*80)\n"
"        print(f\"{'SERVICE':<30} {'STATUS':<15} {'STATE':<15}\")\n"
"        print(\"=\"*80)\n"
"        \n"
"        count = 0\n"
"        for s in self.services:\n"
"            if filter_active and s['active'] != 'active':\n"
"                continue\n"
"            if filter_inactive and s['active'] == 'active':\n"
"                continue\n"
"            \n"
"            status = \"● ACTIVE\" if s['active'] == 'active' else \"○ STOPPED\"\n"
"            print(f\"{s['name'][:29]:<30} {status:<15} {s['sub'][:14]:<15}\")\n"
"            count += 1\n"
"            if count >= 20:\n"
"                print(\"\\n... and other\")\n"
"                break\n"
"    \n"
"    def manage_service(self):\n"
"        service = input(\"Enter a Service Name: \").strip()\n"
"        if not service:\n"
"            return\n"
"        \n"
"        print(f\"\\nControl {service}:\")\n"
"        print(\"[1] Launch\")\n"
"        print(\"[2] Stop\")\n"
"        print(\"[3] Relaunch\")\n"
"        print(\"[4] Enable Autoload\")\n"
"        print(\"[5] Disable Autoload\")\n"
"        \n"
"        action = input(\"\\nChoice an Option: \").strip()\n"
"        \n"
"        commands = {\n"
"            '1': 'start', '2': 'stop', '3': 'restart',\n"
"            '4': 'enable', '5': 'disable'\n"
"        }\n"
"        \n"
"        if action in commands:\n"
"            print(f\"[*] Working: {commands[action]} {service}\")\n"
"            try:\n"
"                result = subprocess.run(['sudo', 'systemctl', commands[action], service],\n"
"                                      capture_output=True, text=True)\n"
"                \n"
"                if result.returncode == 0:\n"
"                    print(f\"[+] Successfully!\")\n"
"                else:\n"
"                    print(f\"[-] Error: {result.stderr}\")\n"
"            except Exception as e:\n"
"                print(f\"[-] Error: {e}\")\n"
"    \n"
"    def show_logs(self):\n"
"        service = input(\"Enter a Service Name: \").strip()\n"
"        if service:\n"
"            lines = input(\"Number of lines (default 20): \").strip() or \"20\"\n"
"            try:\n"
"                subprocess.run(['sudo', 'journalctl', '-u', service, '-n', lines, '--no-pager'])\n"
"            except Exception as e:\n"
"                print(f\"[-] Error: {e}\")\n"
"\n"
"if name == \"main\":\n"
"    if os.geteuid() != 0:\n"
"        print(\"[!] Some functions need root\")\n""    \n"
"    manager = ServiceManager()\n"
"    manager.show_menu()\n";
        
        run_python_script(script);
    }
    
    void wifi_scanner() {
        cout << CYAN << "[*] Launch WiFi Scanner..." << RESET << endl;
        
        const char* script = 
"#!/usr/bin/env python3\n"
"import subprocess\n"
"import re\n"
"import os\n"
"\n"
"def scan_wifi():\n"
"    print(\"\"\"\n"
"    ╔══════════════════════════════╗\n"
"    ║       WiFi SCANNER v1.0      ║\n"
"    ╚══════════════════════════════╝\n"
"    \"\"\")\n"
"    \n"
"    try:\n"
"        result = subprocess.run(['iwconfig'], capture_output=True, text=True)\n"
"        interfaces = re.findall(r'^(\\w+).*?IEEE 802.11', result.stdout, re.MULTILINE)\n"
"    except:\n"
"        print(\"[-] Failed to execute iwconfig\")\n"
"        return\n"
"    \n"
"    if not interfaces:\n"
"        print(\"[-] WiFi Interface Not Found!\")\n"
"        return\n"
"    \n"
"    iface = interfaces[0]\n"
"    print(f\"[+] Using interface: {iface}\")\n"
"    \n"
"    try:\n"
"        subprocess.run(['sudo', 'ip', 'link', 'set', iface, 'up'], check=True)\n"
"    except:\n"
"        print(f\"[-] Failed to bring up interface {iface}\")\n"
"        return\n"
"    \n"
"    print(\"[*] Network scanning...\")\n"
"    print(\"[*] This may take a few seconds....\\n\")\n"
"    \n"
"    try:\n"
"        result = subprocess.run(['sudo', 'iw', 'dev', iface, 'scan'],\n"
"                              capture_output=True, text=True, timeout=30)\n"
"        \n"
"        networks = []\n"
"        current_net = {}\n"
"        \n"
"        for line in result.stdout.split('\\n'):\n"
"            if 'SSID:' in line:\n"
"                if current_net:\n"
"                    networks.append(current_net)\n"
"                ssid = line.split('SSID:')[1].strip()\n"
"                if ssid:\n"
"                    current_net = {'ssid': ssid}\n"
"                else:\n"
"                    current_net = {'ssid': '<Hidden SSID>'}\n"
"            elif 'signal:' in line and current_net:\n"
"                signal = re.findall(r'[-]?\\d+', line)\n"
"                if signal:\n"
"                    current_net['signal'] = int(signal[0])\n"
"            elif 'freq:' in line and current_net:\n"
"                freq = re.findall(r'\\d+', line)\n"
"                if freq:\n"
"                    current_net['channel'] = freq[0]\n"
"            elif 'Encryption key:off' in line and current_net:\n"
"                current_net['encrypted'] = False\n"
"            elif 'Encryption key:on' in line and current_net:\n"
"                current_net['encrypted'] = True\n"
"        \n"
"        if current_net:\n"
"            networks.append(current_net)\n"
"        \n"
"        if networks:\n"
"            print(f\"[+] Networks found: {len(networks)}\\n\")\n"
"            print(\"-\" * 80)\n"
"            print(f\"{'SSID':<30} {'Signal':<8} {'Channel':<6} {'Encryption':<10}\")\n"
"            print(\"-\" * 80)\n"
"            \n"
"            # Sort by signal\n"
"            networks.sort(key=lambda x: x.get('signal', -100), reverse=True)\n"
"            \n"
"            for net in networks:\n"
"                signal = net.get('signal', -100)\n"
"                signal_bar = \"\"\n"
"                if signal > -50:\n"
"                    signal_bar = \"▂▃▄▅▆▇█\"\n"
"                elif signal > -60:\n"
"                    signal_bar = \"▂▃▄▅▆▇\"\n"
"                elif signal > -70:\n"
"                    signal_bar = \"▂▃▄▅▆\"\n"
"                else:\n"
"                    signal_bar = \"▂▃▄\"\n"
"                \n"
"                encrypted = \"Yes\" if net.get('encrypted', True) else \"No\"\n"
"                print(f\"{net['ssid'][:29]:<30} {signal_bar:8} {net.get('channel', '?'):6} {encrypted:10}\")\n""        else:\n"
"            print(\"[-] Networks Not Found!\")\n"
"            \n"
"    except subprocess.TimeoutExpired:\n"
"        print(\"[-] Scan Timeout\")\n"
"    except Exception as e:\n"
"        print(f\"[-] Error: {e}\")\n"
"\n"
"if name == \"main\":\n"
"    try:\n"
"        scan_wifi()\n"
"    except KeyboardInterrupt:\n"
"        print(\"\\n[!] Scanning interrupted\")\n";
        
        run_python_script(script);
        pauseScreen();
    }
    
    void password_audit() {
        cout << CYAN << "[*] Launching Password Audit..." << RESET << endl;
        
        const char* script = 
"#!/usr/bin/env python3\n"
"import hashlib\n"
"import re\n"
"import sys\n"
"\n"
"try:\n"
"    import requests\n"
"except ImportError:\n"
"    print(\"[!] requests library not installed\")\n"
"    print(\"[!] Install: sudo apt install python3-requests\")\n"
"    requests = None\n"
"\n"
"def check_password_strength(password):\n"
"    score = 0\n"
"    feedback = []\n"
"    \n"
"    if len(password) >= 12:\n"
"        score += 2\n"
"        feedback.append(\"✓ Excellent length\")\n"
"    elif len(password) >= 8:\n"
"        score += 1\n"
"        feedback.append(\"✓ Good length\")\n"
"    else:\n"
"        feedback.append(\"✗ Password too short (minimum 8 characters)\")\n"
"    \n"
"    if re.search(r'[A-Z]', password):\n"
"        score += 1\n"
"        feedback.append(\"✓ Has uppercase letters\")\n"
"    else:\n"
"        feedback.append(\"✗ No uppercase letters\")\n"
"    \n"
"    if re.search(r'[a-z]', password):\n"
"        score += 1\n"
"        feedback.append(\"✓ Has lowercase letters\")\n"
"    else:\n"
"        feedback.append(\"✗ No lowercase letters\")\n"
"    \n"
"    if re.search(r'\\d', password):\n"
"        score += 1\n"
"        feedback.append(\"✓ Has numbers\")\n"
"    else:\n"
"        feedback.append(\"✗ No numbers\")\n"
"    \n"
"    if re.search(r'[!@#$%^&*(),.?\":{}|<>]', password):\n"
"        score += 1\n"
"        feedback.append(\"✓ Has special characters\")\n"
"    else:\n"
"        feedback.append(\"✗ No special characters\")\n"
"    \n"
"    common_words = ['password', '123456', 'qwerty', 'admin', 'letmein', 'welcome', 'password123']\n"
"    if password.lower() in common_words:\n"
"        score = 0\n"
"        feedback = [\"✗ Password is too common (dictionary word)!\"]\n"
"    \n"
"    return score, feedback\n"
"\n"
"def check_pwned_password(password):\n"
"    if not requests:\n"
"        return False\n"
"    \n"
"    sha1 = hashlib.sha1(password.encode()).hexdigest().upper()\n"
"    prefix, suffix = sha1[:5], sha1[5:]\n"
"    \n"
"    try:\n"
"        response = requests.get(f'https://api.pwnedpasswords.com/range/{prefix}', timeout=5)\n"
"        if response.status_code == 200:\n"
"            hashes = [line.split(':')[0] for line in response.text.splitlines()]\n"
"            if suffix in hashes:\n"
"                return True\n"
"    except:\n"
"        pass\n"
"    return False\n"
"\n"
"def main():\n"
"    print(\"\"\"\n"
"    ╔══════════════════════════════╗\n"
"    ║    PASSWORD AUDIT v1.0       ║\n"
"    ╚══════════════════════════════╝\n"
"    \"\"\")\n"
"    \n"
"    while True:\n"
"        password = input(\"Enter password to check (or 'exit' to quit): \")\n"
"        if password.lower() == 'exit':\n"
"            break\n"
"        \n"
"        if not password:\n"
"            continue\n"
"        \n"
"        print(\"\\n\" + \"=\"*50)\n"
"        print(\"CHECK RESULTS:\")\n"
"        print(\"=\"*50)\n"
"        \n"
"        score, feedback = check_password_strength(password)\n"
"        \n"
"        for item in feedback:\n""            print(item)\n"
"        \n"
"        print(f\"\\nStrength score: {score}/6\")\n"
"        \n"
"        if score <= 2:\n"
"            print(\"Very weak password!\")\n"
"        elif score <= 4:\n"
"            print(\"Medium password\")\n"
"        else:\n"
"            print(\"Strong password!\")\n"
"        \n"
"        if requests:\n"
"            print(\"\\n[*] Checking breach database...\")\n"
"            if check_pwned_password(password):\n"
"                print(\"WARNING: This password found in data breaches!\")\n"
"            else:\n"
"                print(\"Password not found in known breaches\")\n"
"        \n"
"        print()\n"
"\n"
"if name == \"main\":\n"
"    try:\n"
"        main()\n"
"    except KeyboardInterrupt:\n"
"        print(\"\\n[!] Exiting\")\n"
"        sys.exit(0)\n";
        
        run_python_script(script);
    }
};

void showStatus(string text, string color) {
    cout << color << "[*] " << text << RESET << endl;
}

void statusInfo(string text) { showStatus(text, CYAN); }
void statusWork(string text) { showStatus(text, YELLOW); }
void statusOk(string text) { showStatus(text, GREEN); }
void statusError(string text) { showStatus(text, RED); }

void pauseScreen() {
    cout << YELLOW << "\nPress Enter to Continue..." << RESET;
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    cin.get();
}

void slowPrint(const std::string& text, int delayMs) {
    for (char c : text) {
        cout << c << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(delayMs));
    }
    cout << std::endl;
}

void showMenu() {
    system("clear");
    slowPrint("hello, friend...", 50);
    
    std::this_thread::sleep_for(std::chrono::seconds(1));

    system("clear");
    
    cout << MAGENTA;
    cout << "╔════════════════════════════════════╗\n";
    cout << "║        KALI HELPER 0.9.0           ║\n";
    cout << "║       NEW: Python Scripts!         ║\n";
    cout << "╚════════════════════════════════════╝\n";
    cout << RESET;
    
    cout << GREEN << "[1] " << RESET << "Scan Network (Python)" << endl;
    cout << GREEN << "[2] " << RESET << "System Monitor (Python)" << endl;
    cout << GREEN << "[3] " << RESET << "Services (Python)" << endl;
    cout << GREEN << "[4] " << RESET << "WiFi Scan (Python)" << endl;
    cout << GREEN << "[5] " << RESET << "Password audit (Python)" << endl;
    cout << GREEN << "[6] " << RESET << "Base Kali Tools (C++)" << endl;
    cout << GREEN << "[7] " << RESET << "OSINT Toolkit (C++)" << endl;
    cout << GREEN << "[8] " << RESET << "devices (ESP/Flipper)" << endl;
    cout << GREEN << "[9] " << RESET << "Program Info" << endl;
    cout << RED   << "[10] " << RESET << "Donate me" << endl;
    cout << RED   << "[0] " << RESET << "Exit\n" << endl;
}

int getChoice() {
    int choice;
    cout << BLUE << "Choice: " << RESET;
    
    while (!(cin >> choice)) {
        cin.clear();
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        cout << RED << "Incorrect choice. Try again: " << RESET;
    }
    
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return choice;
}

void webTest() { 
    statusWork("Installing Web tools..."); 
    system("sudo apt install kali-tools-web -y"); 
    statusOk("Done."); 
    pauseScreen();
}

void exploitation() { 
    statusWork("Installing Exploitation tools..."); 
    system("sudo apt install kali-tools-exploitation -y"); 
    statusOk("Done."); 
    pauseScreen();
}

void sniffingSpoofing() { 
    statusWork("Installing Sniffing tools..."); 
    system("sudo apt install kali-tools-sniffing-spoofing -y"); 
    statusOk("Done."); 
    pauseScreen();
}

void information() { 
    statusWork("Installing Information Gathering Tools..."); 
    system("sudo apt install kali-tools-information-gathering -y"); 
    statusOk("Done."); 
    pauseScreen();
}void kaliTools() {
    system("clear");
    
    cout << CYAN << "Update Repos? (Y/N): " << RESET;
    char ch;
    cin >> ch;
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    if (ch == 'y' || ch == 'Y') {
        statusWork("Updating...");
        system("sudo apt update");
        statusOk("Done.");
    }
    
    int toolChoice;
    cout << "\n[1] Web\n[2] Exploitation\n[3] Sniffing\n[4] Information\n[0] Back\n";
    cout << BLUE << "Choice: " << RESET;
    cin >> toolChoice;
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    switch (toolChoice) {
        case 1: webTest(); break;
        case 2: exploitation(); break;
        case 3: sniffingSpoofing(); break;
        case 4: information(); break;
        default: break;
    }
}

void installOsintTools() {
    statusWork("Installing OSINT tools...");
    system("sudo apt install sherlock theharvester whois maltego -y");
    statusOk("Done.");
    pauseScreen();
}

void usernameSearch() {
    string user;
    cout << "Enter username: ";
    cin >> user;
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    statusWork("Searching...");
    string command = "sherlock " + user;
    system(command.c_str());
    pauseScreen();
}

void programmInfo()
{
    system("clear");
    cout << "==== INFO ====\n";
    cout << "Last update: The program's structure has been radically changed;\n";
    cout << "most of the functionality is now written in Python.\n";
    cout << "This makes it easy to troubleshoot issues,\n";
    cout << "and also allows beginners to customize the program's functionality to suit their needs.\n";

    string choice;

    cout << "Do you want to know what the next update will be? (Y/N)\n";

    cin >> choice;

    if (choice == "Y" || choice == "y")
    {
        cout << "The next GLOBAL update will be related to ESP32." << std::endl;
        pauseScreen();
    } 
    else if (choice == "N" || choice == "n")
    {
        pauseScreen();
    } else {
        cout << "Incorrect Choice!" << endl;
        pauseScreen();
    }
}

void domainInfo() {
    string domain;
    cout << "Enter domain: ";
    cin >> domain;
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    statusWork("Getting information...");
    string command = "whois " + domain;
    system(command.c_str());
    pauseScreen();
}

void osintMenu() {
    int choice;
    do {
        system("clear");
        cout << CYAN << "====== OSINT TOOLKIT ======\n" << RESET;
        cout << "[1] Install OSINT Toolkit\n";
        cout << "[2] Username Search (Sherlock)\n";
        cout << "[3] Domain Info (whois)\n";
        cout << "[0] Go Back\n";
        cout << BLUE << "Choice: " << RESET;
        
        cin >> choice;
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        
        switch (choice) {
            case 1: installOsintTools(); break;
            case 2: usernameSearch(); break;
            case 3: domainInfo(); break;
        }
    } while (choice != 0);
}

void connectEsp() {
    system("clear");

    cout << BLUE << "=== ESP32 CONNECTION ===" << RESET << endl;
    cout << YELLOW << "Enter Port (example, /dev/ttyUSB0): ";

    string port;
    cin >> port;

    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    cout << GREEN << "Connecting to " + port + "..." << RESET << endl;

    pauseScreen();
}

void flipperZero() {
    statusWork("In Development...\n");
    pauseScreen();
}

void deviceMenu() {
    system("clear");
    int choice;
    
    cout << GREEN << "[1] " << RESET << "ESP32\n";
    cout << GREEN << "[2] " << RESET << "Flipper Zero\n";
    cout << BLUE << "Choice: " << RESET;
    cin >> choice;
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    switch (choice) {
        case 1: connectEsp(); break;
        case 2: flipperZero(); break;
        default:
            cout << RED << "Incorrect Choice!\n" << RESET;
            pauseScreen();
    }
}

void donate()
{
    cout << "Soon..." << endl;
    pauseScreen();
}int main() {
    PythonScripts py;
    
    if (geteuid() != 0) {
        cout << YELLOW << "[!] Some functions require root rights\n" << RESET;
        cout << YELLOW << "[!] Run with: sudo ./kali_helper\n" << RESET;
    }
    
    while (true) {
        showMenu();
        statusOk("Ready to Work.");
        
        switch (getChoice()) {
            case 1: py.network_scan(); break;
            case 2: py.system_monitor(); break;
            case 3: py.service_manager(); break;
            case 4: py.wifi_scanner(); break;
            case 5: py.password_audit(); break;
            case 6: kaliTools(); break;
            case 7: osintMenu(); break;
            case 8: deviceMenu(); break;
            case 9: programmInfo(); break;
            case 10: donate(); break;
            case 0: 
                cout << GREEN << "Good Luck!\n" << RESET;
                exit(0);
            default:
                cout << RED << "Incorrect choice\n" << RESET;
                pauseScreen();
        }
    }
    
    return 0;
}