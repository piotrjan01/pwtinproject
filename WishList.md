<a href='Hidden comment: 
#sidebar WishListTOC
wiki.l11n
'></a>




---

# TODO #
## Config ##
  * command-line options with `getopt'
  * debug level from
    1. config
    1. command-line
  * local IP automatic resolving


---

## Protocols ##
### SIP ###
  * implement other message types

### RTP ###
#### OUTpackets ####
  * set _Mark_ flag in the first packet

#### INpackets ####
  * validate if RTP Header is well-formed
  * order packets according to _sequenceNumber_

### RTCP ###
  * implement


---

## LACK ##
Design header for steg. data, that will meet following needs:
  * add data checksum and sequence number for steg. data
  * send ACK with sequence number
  * receive ACK and eventually retransmit
  * sort data using sequence number (similar need as in RTP)