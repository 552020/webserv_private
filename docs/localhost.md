# localhost

`http://127.0.0.1:8080` or `http://localhost:8080`

The IP Address: 127.0.0.1 is the loopback Internet Protocol (IP) address also used to refer to your own computer. It’s part of the loopback network (127.0.0.0/8 IP block), which is reserved by the Internet Engineering Task Force (IETF) for loopback purposes. Any IP address starting with 127. is loopback, but 127.0.0.1 is the most commonly used.

`http://localhost:8080`

## Reserved IP Adresses ranges

The Internet Engineering Task Force (IETF) has reserved several IP address blocks for various testing, documentation, and educational purposes, beyond just the loopback address range (`127.0.0.0/8`).

- **Loopback Addresses (`127.0.0.0/8`)**: This block contains 16,777,214 addresses (`127.0.0.1` to `127.255.255.254`) used for loopback purposes, allowing a device to send messages to itself.

- **Private Networks (`10.0.0.0/8`, `172.16.0.0/12`, `192.168.0.0/16`)**: These ranges are set aside for use in private networks. They can be used by anyone setting up internal networks and are not routed on the global internet.

- **Link-Local Addresses (`169.254.0.0/16`)**: These are used for automatic IP address assignment on a local network when no external DHCP server is available.
- **Test-Net Addresses (`192.0.2.0/24`, `198.51.100.0/24`, `203.0.113.0/24`)**: Designated for use in documentation and network examples to avoid conflicts with real internet addresses.
- **Multicast Addresses (`224.0.0.0/4`)**: Reserved for multicast groups (used to send a single packet to multiple destinations).
- **Future Use and Research (`240.0.0.0/4`)**: This block was originally reserved for future use, though parts of it are now being reconsidered for various purposes, including research.

### Rationale Behind the Reservations

1. **Future-Proofing**: The IETF made these reservations not just based on the current needs but also considering future requirements and the evolution of network technologies.
2. **Avoiding Conflicts**: By setting aside blocks for documentation, testing, and private networks, the IETF aimed to prevent conflicts that could arise from overlapping use of IP addresses in different scenarios.
3. **Encouraging Innovation**: Reserving blocks for research and future use allows for experimentation with new networking technologies and protocols without interfering with the public internet.
4. **Global Uniformity**: Having globally recognized reserved blocks facilitates consistency in networking practices, documentation, and educational materials.
