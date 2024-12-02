#include <sys/errno.h>
#include <sys/param.h>
#include <sys/module.h>
#include <sys/kernel.h>

#include <linux/skbuff.h>
#include <linux/tcp.h>
#include <linux/ip.h>
#include <linux/if_ether.h>

static struct sk_buff *ln_tx_path(void);
static void ln_rx_path(struct sk_buff *);
static void print_skb(struct sk_buff *);

static int __init ln_skb_init(void) 
{
	struct sk_buff *skb = NULL;
	printf("module loaded!!\n");

	skb = ln_tx_path();
	if (!skb) {
                printf("Failed to allocate");
                return -ENOMEM;
        }

	ln_rx_path(skb);

	if (skb) {
		kfree_skb(skb);
		skb = NULL;
	}

	return 0;
}

static void print_skb(struct sk_buff *skb)
{
	int i = 0;

	if(!skb) {
		printf("Failed! skb is null");
		return;
	}
	if(!skb->len) {
		printf("Failed! No data");
		return;
	}

	for (i=0; i<skb->len; i++)
                printf("%02hhx ", skb->data[i]);
}

static struct sk_buff *ln_tx_path(void) 
{
	struct sk_buff *skb = NULL;
	struct tcphdr *tcp_hdr;
	struct iphdr *ip_hdr;
	struct ethhdr *eth_hdr;
	char *skb_data;
	char *fcs;
	int len = 256;
	int head_len = 128;
	int data_len = 11;

	printf("Entry %s:%d\n", __func__, __LINE__);
	skb = alloc_skb(len, GFP_KERNEL);
	if (!skb) {
                printf("Failed to allocate skb");
                return NULL;
        }

	printf("===========================\n");
	printf("skb=%p skb->len=%d\n", skb, skb->len);
	printf("Headroom : %d\n", skb_headroom(skb));
	printf("Tailroom : %d\n", skb_tailroom(skb));
	printf("===========================\n");

	skb_reserve(skb, head_len);
	printf("\n==== After skb_reserve ====\n");
	printf("skb=%p skb->len=%d\n", skb, skb->len);
	printf("Headroom : %d\n", skb_headroom(skb));
	printf("Tailroom : %d\n", skb_tailroom(skb));
	printf("============================\n");

	skb_data = skb_put(skb, data_len);
	memcpy(skb_data, "Hello world", data_len);
	printf("\n==== After skb_put ====\n");
	printf("skb=%p skb->len=%d\n", skb, skb->len);
	printf("Headroom : %d\n", skb_headroom(skb));
	printf("Tailroom : %d\n", skb_tailroom(skb));
	printf("skb->data=%p skb->data=%s\n", skb->data, skb->data);
	print_skb(skb);
	printf("\n============================\n");

	tcp_hdr = skb_push(skb, sizeof(struct tcphdr));
	memset(tcp_hdr, 'T' , sizeof(struct tcphdr));
	printf("\n==== After skb_push TCP hdr ====\n");
	printf("skb=%p skb->len=%d\n", skb, skb->len);
	printf("Headroom : %d\n", skb_headroom(skb));
	printf("Tailroom : %d\n", skb_tailroom(skb));
	printf("skb->data=%p \n", skb->data);
	print_skb(skb);
	printf("\n============================\n");

	ip_hdr = skb_push(skb, sizeof(struct iphdr));
	memset(ip_hdr, 'I' , sizeof(struct iphdr));
	printf("\n==== After skb_push IP hdr ====\n");
	printf("skb=%p skb->len=%d\n", skb, skb->len);
	printf("Headroom : %d\n", skb_headroom(skb));
	printf("Tailroom : %d\n", skb_tailroom(skb));
	printf("skb->data=%p \n", skb->data);
	print_skb(skb);
	printf("\n============================\n");

	eth_hdr = skb_push(skb, sizeof(struct ethhdr));
	memset(eth_hdr, 'E' , sizeof(struct ethhdr));
	printf("\n==== After skb_push ETH hdr ====\n");
	printf("skb=%p skb->len=%d\n", skb, skb->len);
	printf("Headroom : %d\n", skb_headroom(skb));
	printf("Tailroom : %d\n", skb_tailroom(skb));
	printf("skb->data=%p \n", skb->data);
	print_skb(skb);
	printf("\n============================\n");

	fcs = skb_put(skb, 4);
	memcpy(fcs, "fcs", 4);
	printf(  "\n==== After skb_put FCS ====\n");
	printf(  "skb=%p skb->len=%d\n", skb, skb->len);
	printf(  "Headroom : %d\n", skb_headroom(skb));
	printf(  "Tailroom : %d\n", skb_tailroom(skb));
	printf(  "skb->data=%p \n", skb->data);
	print_skb(skb);
	printf(  "\n============================\n");

	printf(  "Exit %s:%d skb=%p\n", __func__, __LINE__, skb);
	return skb;
}

static void ln_rx_path(struct sk_buff *skb)
{
	int fcs_len = 4;
	printf("Entry %s:%d\n", __func__, __LINE__);

	printf("%s: ===== SKB Data len=%d =====\n",__func__, skb->len);
	print_skb(skb);

	printf("%s: ===== SKB fcs=%s =====\n",__func__, skb->data+(skb->len - fcs_len));

	if (memcmp(skb->data+(skb->len - fcs_len), "fcs", fcs_len) != 0) {
		printf(  "%s: FCS check failed\n", __func__);
		return;
	}

	skb_trim(skb, 65);
	printf(  "\n==== After skb_trim FCS ====\n");
	printf(  "skb=%p skb->len=%d\n", skb, skb->len);
	printf(  "Headroom : %d\n", skb_headroom(skb));
	printf(  "Tailroom : %d\n", skb_tailroom(skb));
	printf(  "skb->data=%p \n", skb->data);
	print_skb(skb);
	printf(  "\n============================\n");

	skb_pull(skb, sizeof(struct ethhdr));
	printf(  "\n==== After skb_pull ETH hdr ====\n");
	printf(  "skb=%p skb->len=%d\n", skb, skb->len);
	printf(  "Headroom : %d\n", skb_headroom(skb));
	printf(  "Tailroom : %d\n", skb_tailroom(skb));
	printf(  "skb->data=%p \n", skb->data);
	print_skb(skb);
	printf(  "\n============================\n");

	skb_pull(skb, sizeof(struct iphdr));
	printf(  "\n==== After skb_pull IP hdr ====\n");
	printf(  "skb=%p skb->len=%d\n", skb, skb->len);
	printf(  "Headroom : %d\n", skb_headroom(skb));
	printf(  "Tailroom : %d\n", skb_tailroom(skb));
	printf(  "skb->data=%p \n", skb->data);
	print_skb(skb);
	printf(  "\n============================\n");

	skb_pull(skb, sizeof(struct tcphdr));
	printf(  "\n==== After skb_pull TCP hdr ====\n");
	printf(  "skb=%p skb->len=%d\n", skb, skb->len);
	printf(  "Headroom : %d\n", skb_headroom(skb));
	printf(  "Tailroom : %d\n", skb_tailroom(skb));
	printf(  "skb->data=%p \n", skb->data);
	print_skb(skb);
	printf(  "\n============================\n");

	printf(  "Exit %s:%d\n", __func__, __LINE__);
}


static void __exit ln_skb_exit(void)
{
	printf(  "module unloaded\n");
}

static int
bsdmbuf_loader(struct module *m, int what, void *arg)
{
        int err = 0;

        switch (what) {
        case MOD_LOAD:
                printf("==== bsdmbuf loaded ====\n");
                ln_skb_init();
                break;
        case MOD_UNLOAD:
                printf("==== bsdmbuf unloaded ====\n");
		ln_skb_exit();
                break;
        default:
                err = EOPNOTSUPP;
                break;
        }
        return(err);
}

static moduledata_t bsdmbuf_mod = {
        "bsdmbuf",
        bsdmbuf_loader,
        NULL
};

DECLARE_MODULE(freebsd_mbuf, bsdmbuf_mod, SI_SUB_KLD, SI_ORDER_ANY);
