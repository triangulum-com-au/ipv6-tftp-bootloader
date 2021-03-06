#include <np.h>     /* NovaProva library */
#include "../target/icmpv6.h" /* declares the Code Under Test */

#define MY_IP    0xfe, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x1e, 0xc0, 0xff, 0xfe, 0x81, 0xf9, 0x1b
#define OTHER_IP 0xfe, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf2, 0xde, 0xf1, 0xff, 0xfe, 0x77, 0x6a, 0xc6
#define MY_ETH 0x00, 0x1e, 0xc0, 0x81, 0xf9, 0x1b
#define OTHER_ETH 0xf0, 0xde, 0xf1, 0x77, 0x6a, 0xc6
const static uint8_t ip_ping_req[] =  {
  0x60, 0x00, 0x00, 0x00, 0x00, 0x40, 0x3a, 0x40, 
  OTHER_IP, 
  MY_IP };
const static uint8_t icmp_ping_req[] = {
  0x80, 0x00, 0xf8, 0xac, 0x28, 0xd5, 0x00, 0x01, 0x36, 0x8d, 0x03, 0x55, 0x00, 0x00, 0x00, 0x00,
  0x52, 0x72, 0x0d, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
  0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27,
  0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37};

  
const static uint8_t ip_ping_expected[] =  {
  0x60, 0x00, 0x00, 0x00, 0x00, 0x40, 0x3a, 0xff, 
  MY_IP, 
  OTHER_IP };
  
const static uint8_t icmp_ping_expected[] = {
 0x81, 0x00, 0xf7, 0xac, 0x28, 0xd5, 0x00, 0x01, 0x36, 0x8d, 0x03, 0x55, 0x00, 0x00, 0x00, 0x00,
 0x52, 0x72, 0x0d, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27,
 0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37};

 
const static uint8_t ip_ns_req[] =  {
  0x60, 0x00, 0x00, 0x00, 0x00, 0x20, 0x3a, 0x40, 
  OTHER_IP, 
  MY_IP };
  
const static uint8_t ip_ns_expected[] =  {
  0x60, 0x00, 0x00, 0x00, 0x00, 0x20, 0x3a, 0xff, 
  MY_IP, 
  OTHER_IP };
  
const static uint8_t icmp_ns_req[] = { 
 0x87, 0x00, 0x6b, 0x6f, 0x00, 0x00, 0x00, 0x00, 0xfe, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x02, 0x1e, 0xc0, 0xff, 0xfe, 0x81, 0xf9, 0x1b, 0x01, 0x01, OTHER_ETH};

const static uint8_t icmp_ns_expected[] = {
 0x88, 0x00, 0x9c, 0xd0, 0x60, 0x00, 0x00, 0x00, 0xfe, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x02, 0x1e, 0xc0, 0xff, 0xfe, 0x81, 0xf9, 0x1b, 0x02, 0x01, MY_ETH};
 
static void dump( void *a, void *b, int length ){
  int i = 0;
  puts("---");
  for( i = 0 ; i < length; i++ ) {
    printf("%02x %02x\n", ((uint8_t *)a)[i] , ((uint8_t *)b)[i] );
  }
}

static void test_icmpv6_dispatch_ping(void)
{
  struct ip_packet pkt;
  uint8_t me[IPV6_ADDR_LENGTH] = { MY_IP };

  ipv6_config_set_address( me ) ;
  
  memcpy( &pkt.header, ip_ping_req, sizeof( ip_ping_req ) );
  struct icmpv6_ping_packet *ping = (struct icmpv6_ping_packet *)(pkt.payload);
  memcpy( ping, icmp_ping_req, sizeof(icmp_ping_req) );

  int result = icmpv6_dispatch( &pkt );
  
  NP_ASSERT_EQUAL(sizeof(icmp_ping_expected), result);
  NP_ASSERT_EQUAL( 0 , memcmp(  ip_ping_expected, &pkt.header, sizeof( ip_ping_expected ) ) );  
  NP_ASSERT_EQUAL( 0 , memcmp(  icmp_ping_expected, ping,  sizeof( icmp_ping_expected ) ) );
}

static void test_icmpv6_dispatch_ns(void)
{
  struct ip_packet pkt;
  uint8_t me[IPV6_ADDR_LENGTH] = { MY_IP };
  uint8_t them[IPV6_ADDR_LENGTH] = { OTHER_IP };
  uint8_t my_eth[ETH_ADDR_LENGTH] = { MY_ETH };
  uint8_t expected_other_eth[ETH_ADDR_LENGTH] = { OTHER_ETH };
  uint8_t *other_eth = NULL;
  
  ipv6_config_set_address( me ) ;
  eth_config_set_address( my_eth );
  
  memcpy( &pkt.header, ip_ns_req, sizeof( ip_ns_req ) );

  struct icmpv6_ns_packet *ns = (struct icmpv6_ns_packet *)(pkt.payload);
  memcpy( ns, icmp_ns_req, sizeof(icmp_ns_req) );
  other_eth = ipv6_physical_address_of(them) ;
  NP_ASSERT_EQUAL( NULL, other_eth);
  
  int result = icmpv6_dispatch( &pkt );
  
  NP_ASSERT_EQUAL(sizeof(icmp_ns_expected), result);
  
  NP_ASSERT_EQUAL( 0 , memcmp(  ip_ns_expected, &pkt.header, sizeof( ip_ns_expected ) ) );  
  NP_ASSERT_EQUAL( 0 , memcmp(  icmp_ns_expected, ns,  sizeof( icmp_ns_expected ) ) );
  
  other_eth = ipv6_physical_address_of(them) ;
  NP_ASSERT_EQUAL( 0, memcmp(  expected_other_eth , other_eth, ETH_ADDR_LENGTH));
  
  /* rfc4861 - 6.1.2 hop limit MUST be 255 */
  NP_ASSERT_EQUAL( 255, pkt.header.hop_limit);
  
}
  