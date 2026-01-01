use std::process::Command;

// Returns (volume, is_muted)
fn get_volume_amixer() -> Result<(u8, bool), Box<dyn std::error::Error>> {
    let output = Command::new("amixer").args(&["get", "Master"]).output()?;
    let output_str = String::from_utf8_lossy(&output.stdout);

    let mut volume = 0;
    let mut is_muted = false;

    for line in output_str.lines() {
        // Check for mute status: [on] or [off]
        if line.contains("[off]") {
            is_muted = true;
        }

        // Parse volume percentage
        if line.contains("%") {
            if let Some(start) = line.find('[') {
                if let Some(end) = line.find('%') {
                    let vol_str = &line[start + 1..end];
                    volume = vol_str.parse()?;
                }
            }
        }
    }

    Ok((volume, is_muted))
}

fn get_volume_pactl() -> Result<(u8, bool), Box<dyn std::error::Error>> {
    let output = Command::new("pactl")
        .args(&["get-sink-volume", "@DEFAULT_SINK@"])
        .output()?;
    let output_str = String::from_utf8_lossy(&output.stdout);

    let mut volume = 0;

    // Parse volume percentage
    for part in output_str.split_whitespace() {
        if part.ends_with('%') {
            let vol_str = part.trim_end_matches('%');
            volume = vol_str.parse()?;
            break;
        }
    }

    // Check mute status
    let mute_output = Command::new("pactl")
        .args(&["get-sink-mute", "@DEFAULT_SINK@"])
        .output()?;
    let mute_str = String::from_utf8_lossy(&mute_output.stdout);
    let is_muted = mute_str.contains("yes");

    Ok((volume, is_muted))
}

pub fn get_volume() -> Result<(u8, bool), Box<dyn std::error::Error>> {
    get_volume_pactl().or_else(|_| get_volume_amixer())
}
