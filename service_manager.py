#!/usr/bin/env python3
# /home/PythonScripts/service_manager.py

import subprocess
import sys
import os
from typing import List, Dict

class ServiceManager:
    def __init__(self):
        self.services = self.get_all_services()
    
    def get_all_services(self) -> List[Dict]:
        """Get all system services"""
        services = []
        try:
            # Get all services
            result = subprocess.run(['systemctl', 'list-units', '--type=service', '--all', '--no-pager'],
                                  capture_output=True, text=True)
            
            lines = result.stdout.split('\n')
            for line in lines[1:]:  # Skip header
                parts = line.split()
                if len(parts) >= 4 and '.' in parts[0]:
                    services.append({
                        'name': parts[0].replace('.service', ''),
                        'load': parts[1],
                        'active': parts[2],
                        'sub': parts[3],
                        'description': ' '.join(parts[4:])
                    })
        except:
            pass
        
        return services
    
    def print_services(self, filter_active: bool = False):
        """Print services with formatting"""
        print("\n" + "="*80)
        print(f"{'SERVICE':<30} {'STATUS':<15} {'DESCRIPTION':<35}")
        print("="*80)
        
        for service in self.services:
            if filter_active and service['active'] != 'active':
                continue
                
            status_color = ""
            if service['active'] == 'active':
                status = "● ACTIVE"
            elif service['active'] == 'failed':
                status = "✗ FAILED"
            else:
                status = "○ INACTIVE"
            
            print(f"{service['name'][:29]:<30} {status:<15} {service['description'][:34]:<35}")
    
    def control_service(self, service_name: str, action: str) -> bool:
        """Start/stop/restart service"""
        print(f"[*] {action} {service_name}...")
        result = subprocess.run(['sudo', 'systemctl', action, service_name],
                              capture_output=True, text=True)
        
        if result.returncode == 0:
            print(f"[+] Service {action} successful")
            return True
        else:
            print(f"[-] Failed to {action} service: {result.stderr}")
            return False
    
    def check_service_logs(self, service_name: str, lines: int = 20):
        """Show service logs"""
        print(f"[*] Showing last {lines} lines for {service_name}:")
        print("-"*50)
        subprocess.run(['sudo', 'journalctl', '-u', service_name, '-n', str(lines), '--no-pager'])
    
    def interactive_menu(self):
        """Interactive service management menu"""
        while True:
            os.system('clear')
            print("""
    ╔══════════════════════════════╗
    ║     SERVICE MANAGER v1.0     ║
    ╚══════════════════════════════╝
            """)
            
            print("[1] Show all services")
            print("[2] Show only active services")
            print("[3] Start service")
            print("[4] Stop service")
            print("[5] Restart service")
            print("[6] Check service logs")
            print("[0] Exit")
            
            choice = input("\nChoice: ").strip()
            
            if choice == '1':
                self.print_services()
                input("\nPress Enter to continue...")
            elif choice == '2':
                self.print_services(filter_active=True)
                input("\nPress Enter to continue...")
            elif choice in ['3', '4', '5']:
                service = input("Enter service name: ").strip()
                if service:
                    actions = {'3': 'start', '4': 'stop', '5': 'restart'}
                    self.control_service(service, actions[choice])
                    input("\nPress Enter to continue...")
            elif choice == '6':
                service = input("Enter service name: ").strip()
                if service:
                    self.check_service_logs(service)
                    input("\nPress Enter to continue...")
            elif choice == '0':
                break

def main():
    if os.geteuid() != 0:
        print("[!] This script needs root privileges for some operations")
        print("[!] Run with: sudo python3 service_manager.py")
        
    manager = ServiceManager()
    manager.interactive_menu()

if __name__ == "__main__":
    try:
        main()
    except KeyboardInterrupt:
        print("\n[!] Exiting...")
        sys.exit(0)