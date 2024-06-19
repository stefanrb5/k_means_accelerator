#include "hard.hpp"
#include <fstream>

SC_HAS_PROCESS(Hard);

Hard::Hard(sc_module_name name):
	sc_module(name),
	ready(1)
	
	{
		interconnect_socket.register_b_transport(this, &Hard::b_transport);
		SC_REPORT_INFO("Hard", "Constructed.");
	}
	
Hard::~Hard()
{
	SC_REPORT_INFO("Hard", "Destroyed");
}

void Hard::b_transport(pl_t &pl, sc_time &offset)
{
	tlm_command cmd = pl.get_command();
	sc_dt::uint64 addr = pl.get_address();
	unsigned int len = pl.get_data_length();
	unsigned char *buf = pl.get_data_ptr();
	pl.set_response_status(TLM_OK_RESPONSE);
	
	switch(cmd)
	{
		case TLM_WRITE_COMMAND:
			switch(addr)
			{
				case ADDR_ROWS:
					rows = toInt(buf);
					cout << "rows = " << rows << endl;
					break;
				case ADDR_COLS:
					cols = toInt(buf);
					cout << "cols = " << cols << endl;
					break;
				case ADDR_CLUSTER_NUMBERS:
					clusters_number = toInt(buf);
					cout << "clusters_number = " << clusters_number << endl;
					break;	
				case ADDR_START:
					start = toInt(buf);
					cout << "start = " << start << endl;
					findAssociatedCluster(offset);
					break;	
				default:
					pl.set_response_status(TLM_ADDRESS_ERROR_RESPONSE);
					cout << "Wrong address" << endl;
			}
			break;
			
		case TLM_READ_COMMAND:
			switch(addr)
			{
				case ADDR_READY:
					toUchar(buf, ready);
					break;
				default:
					pl.set_response_status(TLM_ADDRESS_ERROR_RESPONSE);
			}
			break;
			
		default:
			pl.set_response_status( tlm::TLM_COMMAND_ERROR_RESPONSE );
			cout << "Wrong command" << endl;
	
	}
	offset += sc_time(DELAY, SC_NS);
}

sc_fixed<16,10> Hard::computeColorDistance(unsigned char pixel[3], unsigned char clusterPixel[3]) {
	sc_fixed<16,16> diffBlue = pixel[0] - clusterPixel[0];
	sc_fixed<16,16> diffGreen = pixel[1] - clusterPixel[1];
	sc_fixed<16,16> diffRed = pixel[2] - clusterPixel[2];

	double diffBlue_double = static_cast<double>(diffBlue);
	double diffGreen_double = static_cast<double>(diffGreen);
	double diffRed_double = static_cast<double>(diffGreen);

	double distance_double = sqrt(pow(diffBlue_double, 2) + pow(diffGreen_double, 2) + pow(diffRed_double, 2));

	sc_fixed<16,10> distance = distance_double;

	return distance;
}

void Hard::findAssociatedCluster(sc_time &system_offset)
{	
	pl_t pl;
	
	unsigned char b;
	unsigned char g;
	unsigned char r;
	
	unsigned char clusterPixel_b;
	unsigned char clusterPixel_g;
	unsigned char clusterPixel_r;
		
	sc_fixed<16,10> distance;
					
	unsigned char *clustersCenters_c = new unsigned char[3 * clusters_number];

	for(int i = 0; i < 3*clusters_number; i++) 	                                           
	{
		clustersCenters_c[i] = read_bram(cols*rows*3 + i);
	}
			
	if (start == 1 && ready == 1)
	{
		ready = 0;
		offset += sc_time(DELAY, SC_NS);
	}
	
	else if (start == 0 && ready == 0)
	{	
		cout << "Processing started" << endl;
		
		for(int i = 0; i < rows; i++)
		{
			for(int j = 0; j < cols * 3; j+= 3) //Prolazimo kroz matricu char-ova, gde su susedna 3 bgr takvim redosledom
			{
				
				sc_uint<9> minDistance = 443;
				unsigned char closestClusterIndex = 0;
				b = read_bram(i*cols*3 + j);
				g = read_bram(i*cols*3 + j + 1); 
				r = read_bram(i*cols*3 + j + 2); 
												
				unsigned char pixel[3] = {b, g, r};

				for(int k = 0; k < clusters_number*3; k+=3)
				{
					clusterPixel_b = clustersCenters_c[k];
					clusterPixel_g = clustersCenters_c[k + 1];
					clusterPixel_r = clustersCenters_c[k + 2];
					
					unsigned char clusterPixel[3] = {clusterPixel_b, clusterPixel_g, clusterPixel_r};
					
					sc_fixed<16,10> distance = computeColorDistance(pixel, clusterPixel); 
					// Update to the closest cluster center
					if (distance < minDistance)
					{
					    minDistance = distance;
					    closestClusterIndex = k/3;
					}	
				}   
            			write_bram(i*cols*3 + j, j/3);
            			write_bram(i*cols*3 + j+1, i);
            			write_bram(i*cols*3 + j+2, closestClusterIndex);    			          			
			}
		}
						    		    				
		cout<<"Upis iz IP u BRAM zavrsen"<<endl;	
					
		ready = 1;
	}	
}


void Hard::write_bram(sc_uint<64> addr, unsigned char val)
{
	pl_t pl;
	unsigned char buf;
	buf = val;
	pl.set_address(addr);
	pl.set_data_length(1); 
	pl.set_data_ptr(&buf);
	pl.set_command( tlm::TLM_WRITE_COMMAND );
	pl.set_response_status ( tlm::TLM_INCOMPLETE_RESPONSE );
	bram_socket->b_transport(pl, offset);
}

unsigned char Hard::read_bram(sc_uint<64> addr)
{
	pl_t pl;
	unsigned char buf;
	pl.set_address(addr);
	pl.set_data_length(1); 
	pl.set_data_ptr(&buf);
	pl.set_command( tlm::TLM_READ_COMMAND );
	pl.set_response_status ( tlm::TLM_INCOMPLETE_RESPONSE );
	bram_socket->b_transport(pl, offset);
	return buf;
}


