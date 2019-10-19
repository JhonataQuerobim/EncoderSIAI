# include <iostream>
# include <fstream>
# include <string>

int main (){
    string teste = "Hello File";

    ofstream resultado;

    resultado.open("resultado.txt");

    resultado << teste << endl;
    return 0;
}