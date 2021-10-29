package interfaces;

import java.util.List;

public interface IUsuario {

	String getNome();

	String getEndereco();

	String getEmail();

	String getCpf();

	List<? extends IVendido> getBensComprados();
	
	List<? extends ILeiloavel> getBensOfertados();
	

}