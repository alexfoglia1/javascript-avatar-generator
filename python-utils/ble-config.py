import subprocess
import scanres

address = scanres.ADDR;

def run_command(command):
    try:
        output = subprocess.check_output(command, shell=True, text=True)
        return output
    except subprocess.CalledProcessError as e:
        return f"Error while executing command: {e}"

command = "rfkill list | grep Bluetooth"  # Sostituisci con il tuo comando
rflist = run_command(command)

device = rflist.split(":")[1];
print("Device: ", device)

if len(address) == 0:
    print("No address found, scanning");
    command = "hcitool scan"
    scanlist = run_command(command)
    scanlist_split = scanlist.split()
    if len(scanlist_split) > 2:
        address = scanlist_split[2]
        print("Found new address: {}".format(address))
        with open("scanres.py", "w") as f:
            f.write("ADDR=\"{}\"".format(address))
    else:
        with open("scanres.py", "w") as f:
            f.write("ADDR=\"\"");
        print("No bluetooth device is found")
        exit()
else:
    print("Using address: {}".format(address))
    
command = "sdptool browse {}".format(address)
channels = run_command(command)
channels_split = channels.split()
spp_slave_idx = -1;
for i in range(len(channels_split) - 2):
    if channels_split[i] == "Name:" and channels_split[i + 1] == "SPP" and channels_split[i + 2] == "slave":
        spp_slave_idx = i + 21;
        break;

if spp_slave_idx == -1:
    print("No SPP slave channel is found")
    exit()
else:
    channel = channels_split[spp_slave_idx]

print("SPP slave channel: {}".format(channel))

command = "rfcomm bind {} {} {}".format(device, address, channel);
bind_res = run_command(command);
print(bind_res);

    
