#include <sys/types.h>
#include <sys/systm.h>
#include <sys/errno.h>
#include <sys/param.h>
#include <sys/module.h>
#include <sys/kernel.h>
#include <sys/mbuf.h>

#include <sys/netinet/tcp.h>
#include <sys/netinet/in.h>
#include <sys/netinet/ip.h>
#include <sys/net/ethernet.h>

static void dump_mbuf_data(struct mbuf *m)
{
	int i;
	int total_len = 0;
	
	if(m == NULL) {
		printf( "Failed! mbuf is NULL\n");
		return;
	}

	while(m != NULL) {
		printf("mbuf: %p\n", m);
		printf("mbuf data: %p\n", m->m_data);
		printf("mbuf length: %d\n", m->m_len);
		printf("mbuf next: %p\n", m->m_next);
		printf("data:\n");
		for (i=0; i < m->m_len; i++) {
			printf("%02x ", *((char *)mtod(m, char *) + i));
			if ((i % 16) == 15) {
				printf("\n");
			}
		}
		printf("\n");

		total_len += m->m_len;
		m = m->m_next;
	}
	
	printf("Total length of data in mbuf chain: %d\n", total_len);
}

static struct mbuf *
bsd_txpath(void)
{
	printf( "Entry %s:%d\n", __func__, __LINE__);

	struct mbuf *m = NULL;
	char *data = "Hello World";
	int data_len = strlen(data);
	int tcphdr_len = sizeof(struct tcphdr);
	int iphdr_len = sizeof(struct ip);
	int ethhdr_len = sizeof(struct ether_header);
	
	// Allocate new mbuf
	m = m_get(M_NOWAIT, MT_DATA);
	if (m == NULL) {
		printf("Failed to allocate mbuf\n");
		return NULL;
	}
	printf("\n======= Allocate mbuf =======\n");
	dump_mbuf_data(m);
	printf("===========================\n");

	// Align data pointer and copy data
	m->m_len = data_len;
	m_align(m, m->m_len);
	memcpy(mtod(m, char*), data, data_len);
	printf("\n======= Copy Data =======\n");
	dump_mbuf_data(m);
	printf("===========================\n");

	// Prepend TCP header
	m = m_prepend(m, tcphdr_len, M_NOWAIT);
	if (m == NULL) {
		printf("No buffer, prepend failed\n");
		return NULL;
	}
	memset(mtod(m, char*), 'T', tcphdr_len);
	printf("\n======= prepend TCP Header =======\n");
	dump_mbuf_data(m);
	printf("===========================\n");

	// Prepend IP header
	m = m_prepend(m, iphdr_len, M_NOWAIT);
	if (m == NULL) {
		printf("No buffer, prepend failed\n");
		return NULL;
	}
	memset(mtod(m, char*), 'I', iphdr_len);
	printf("\n======= prepend IP Header =======\n");
	dump_mbuf_data(m);
	printf("===========================\n");

	// Prepend ETH header
	m = m_prepend(m, ethhdr_len, M_NOWAIT);
	if (m == NULL) {
		printf("No buffer, prepend failed\n");
		return NULL;
	}
	memset(mtod(m, char*), 'E', ethhdr_len);
	printf("\n======= prepend ETH Header =======\n");
	dump_mbuf_data(m);
	printf("===========================\n");

	// Append FCS
	if (m_append(m, 4, "fcs") == 0) {
		printf("mbuf: m_append FCS failed\n");
		return m;
	}
	printf("\n======= append FCS =======\n");
	dump_mbuf_data(m);
	printf("===========================\n");

	return m;	
}

static void
bsd_rxpath(struct mbuf *m)
{
	printf("Entry %s: %d\n", __func__, __LINE__);

	int tcphdr_len = sizeof(struct tcphdr);
	int iphdr_len = sizeof(struct ip);
	int ethhdr_len = sizeof(struct ether_header);
	int fcs_len = 4;
	struct mbuf *last_ch = m;

	printf("\n=========== Rx mbuf ==============\n");
	dump_mbuf_data(m);
	printf("===========================\n");

	// Remove 4 bytes FCS from the last mbuf chain
	while(last_ch != NULL)
	{
		if (last_ch->m_next == NULL) {
			if (last_ch->m_len >= fcs_len) {
				m_adj(m, -fcs_len);
				break;
			}
		}
		last_ch = last_ch->m_next;
	}	
	printf("last_ch=%p\n", last_ch);
	printf("\n======= FCS Remove =========\n");
	dump_mbuf_data(m);
	printf("===========================\n");	

	// Remove ETH header and free the mbuf chain
	m_adj(m, ethhdr_len);
	if (m !=NULL && m->m_len == 0) {
		m = m_free(m);
	}
	printf("\n======= ETH Hdr Remove =========\n");
	dump_mbuf_data(m);
	printf("===========================\n");

	// Remove IP header and free the mbuf chain
	m_adj(m, iphdr_len);
	if (m != NULL && m->m_len == 0) {
		m = m_free(m);
	}
	printf("\n======= IP Hdr Remove =========\n");
	dump_mbuf_data(m);
	printf("===========================\n");

	// Remove TCP header and free the mbuf chain
	m_adj(m, tcphdr_len);
	if (m != NULL && m->m_len == 0) {
		m = m_free(m);
	}
	printf("\n======= TCP Hdr Remove =========\n");
	dump_mbuf_data(m);
	printf("===========================\n");
}

static void
bsd_mbuf_init(void)
{
	printf("bsd_mbuf module loaded\n");
	
	struct mbuf *m = NULL;

	m = bsd_txpath();
	if (m == NULL) {
		printf("mbuf: memory allocation failed\n");
	}

	bsd_rxpath(m);

	if (m) {
		m_freem(m);
		m = NULL;
	}
}

static void
bsd_mbuf_free(void)
{
	printf("bsd_mbuf module unloaded\n");
}

/*
 * Load handler that deals with the loading and unloading of a KLD.
 */
static int
bsd_mbuf_loader(struct module *mod, int what, void *arg)
{
	int err = 0;

	switch (what) {
	case MOD_LOAD:
		bsd_mbuf_init();
		break;
	case MOD_UNLOAD:
		bsd_mbuf_free();
		break;
	default:
		err = EOPNOTSUPP;
		break;
	}
	return(err);
}

/* Declare this module to the rest of the kernel */

static moduledata_t bsd_mbuf_mod = {
	"bsd_mbuf",
	bsd_mbuf_loader,
	NULL
};

DECLARE_MODULE(bsd_mbuf, bsd_mbuf_mod, SI_SUB_KLD, SI_ORDER_ANY);
