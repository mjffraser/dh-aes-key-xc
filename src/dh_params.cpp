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

void DH_Params::set_public_DH_fields(const cpp_int& p, const cpp_int& g) {
	//no changing values after they're set
	if (dh_public_set)
		return;

	_p = p;
	_g = g;

	dh_public_set = true;
}

void DH_Params::set_private_DH_fields(const cpp_int& a, const cpp_int& A, const cpp_int& B, const cpp_int& key) {
	//no changing values after they're set
	if (dh_private_set)
		return;

	_a   = a;
	_A   = A;
	_B   = B;
	_key = key;

	dh_private_set = true;
}

}
