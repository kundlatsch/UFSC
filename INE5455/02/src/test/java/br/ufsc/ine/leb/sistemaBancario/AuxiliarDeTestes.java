package br.ufsc.ine.leb.sistemaBancario;

import br.ufsc.ine.leb.sistemaBancario.*;

public class AuxiliarDeTestes {

    public static ValorMonetario criarVM() {
        return new ValorMonetario(Moeda.BRL);
    }
        
    public static ValorMonetario criarVMPositivo() {
        ValorMonetario vm = new ValorMonetario(Moeda.BRL);
        Dinheiro quantia = new Dinheiro(Moeda.BRL, 50, 0);
        ValorMonetario resultado = vm.somar(quantia);
        return resultado;
    }

    public static ValorMonetario criarVMNegativo() {
        ValorMonetario vm = new ValorMonetario(Moeda.BRL);
        Dinheiro quantia = new Dinheiro(Moeda.BRL, 50, 0);
        ValorMonetario resultado = vm.subtrair(quantia);
        return resultado;
    }
}