from bcc import BPF
from bcc.utils import printb

device = "lo"
b = BPF(src_file="udp_counter.c")
fn = b.load_func("udp_counter", BPF.XDP)
b.attach_xdp(device, fn, 0)

try:
    b.trace_print()
except KeyboardInterrupt:

    dist = b.get_table("counter")
    for k, v in sorted(dist.items()):
        print("DEST_PORT : %10d, COUNT : %10d" % (k.value, v.value))

b.remove_xdp(device, 0)
