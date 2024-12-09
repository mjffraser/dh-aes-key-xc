#pragma once

#include "dh_params.hpp"

namespace dh {

void DH_Params::set_general_fields(std::optional<std::string>& path, std::optional<bool>& db, std::optional<bool>& vp) {
	//no changing values after they're set
	if (general_set)
		return;
	
	if (path)
		log_path = path.value();
	if (db)
		debug_flag = db.value();
	if (vp)
		vetted_primes = vp.value();

	general_set = true;
}

void DH_Params::set_network_fields(bool server_flag, std::optional<unsigned int>& bits_amount, std::optional<std::string>& ip, std::optional<unsigned int>& port_no) {
	//no changing values after they're set
	if (network_set) 
		return;

	server = server_flag;
	if (bits_amount)
		bits = bits_amount.value();
	if (ip)
		ip_addr = ip.value();
	if (port_no)
		port = port_no.value();

	network_set = true;
}

}
