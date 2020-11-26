#define KBUILD_MODNAME "udp_counter"
#include <linux/bpf.h>
#include <linux/if_ether.h>
#include <linux/ip.h>
#include <linux/udp.h>

BPF_HISTOGRAM(counter, u64);

int udp_counter(struct xdp_md *ctx)
{
    bpf_trace_printk("packet received\n");

    void *data = (void *)(long)ctx->data;
    void *data_end = (void *)(long)ctx->data_end;
    struct ethhdr *eth = data;

    if ((void *)eth + sizeof(*eth) <= data_end)
    {

        struct iphdr *ip = data + sizeof(*eth);
        if ((void *)ip + sizeof(*ip) <= data_end)
        {

            if (ip->protocol == IPPROTO_UDP)
            {

                struct udphdr *udp = (void *)ip + sizeof(*ip);
                if ((void *)udp + sizeof(*udp) <= data_end)
                {
                    u64 value = htons(udp->dest);
                    counter.increment(value);
                }
            }
        }
    }
    return XDP_PASS;
}