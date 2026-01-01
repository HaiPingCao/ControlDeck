use serialport::{SerialPortType, available_ports};

pub fn find() -> Option<String> {
    match available_ports() {
        Ok(ports) => {
            for port in ports {
                match &port.port_type {
                    SerialPortType::UsbPort(info) => {
                        if info.vid == 0x2341 || info.vid == 0x1a86 {
                            println!("Found Arduino: {}", port.port_name);
                            return Some(port.port_name);
                        }
                    }
                    _ => {}
                }
            }
            None
        }
        Err(e) => {
            eprintln!("Error listing ports: {}", e);
            None
        }
    }
}
