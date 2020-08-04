//シンプレック法
#include <iostream>
#include <vector>
#include <iomanip>
#include <math.h>
using namespace std;

void printTable(vector<double> coef, vector<vector<double>> rest, vector<double> base, vector<double> b, vector<double> t,vector<double> c, double z, int iteration){
    cout << "Iteracao: " << iteration << endl;
    
    //Cabeçalho
    cout  << " base" << setw(12) << " | ";
    for(int i=0; i<coef.size(); i++)
        cout << setw(9) << "X" << i+1 << "| ";       
    cout << setw(10) << "b|" << setw(11) << "t|" <<endl;
    
    for(int i=0; i<8+(coef.size()+2)*13; i++)
        cout << "-";
    cout << endl;
    //cout<<"-------------------------------------------------------------------------------------------------" <<endl;
    
    //Linhas
    for(int j=0; j<base.size(); j++){
        cout << setw(2) <<"X" << base[j] << setw(11) << coef[base[j]] << setw(3) << " | ";
        for(int i=0; i<coef.size(); i++)
            cout << setw(10) << rest[j][i] << "| "; 
        cout << setw(9) << b[j] << "| " << setw(9) << t[j] << "|" <<endl;
    }  
    
    for(int i=0; i<8+(coef.size()+2)*13; i++)
        cout << "-";
    cout << endl;
    //cout<<"-------------------------------------------------------------------------------------------------" <<endl;
    
    //Cj-Zj
    cout << setw(15) <<"cj-zj" << setw(2) << "| ";
    for(int i=0; i<c.size(); i++)
        cout << setw(10) << c[i] << "| ";
    cout << setw(9) << z << "| " << setw(10) << "|" <<endl;
    
    for(int i=0; i<8+(coef.size()+2)*13; i++)
        cout << "=";
    cout << endl;
    //cout << endl << "=================================================================================================" << endl << endl;
}

int main(){
    //cout << setprecision(4);

 
    
    //Inicializa variavies
    bool minimize = false;
    double z = 0;
    double pivo;
    int num_var, num_rest, num_var_canon = 0;
    vector<double> base, coef, b;
    vector< vector<double> > rest;
    int iteration = 0;
    
    
    //pega os valores
    while(1){
    cout << "Problema de Maximização [Y/N]? ";
    string confirmacao;
    cin >> confirmacao;
    if(confirmacao == "N" || confirmacao == "n" || confirmacao == "no")
        minimize = true;
    else if (confirmacao != "Y" && confirmacao != "y" && confirmacao != "yes")
        cout << "Y ou N" << endl;
    break;
    }
    
    cout << "Entre com o número de coeficientes: ";
    cin >> num_var;
    cout << "Ente com o número de restrições: ";
    cin >> num_rest;
    
    cout << endl << "Entre com os coeficientes das equações na forma padrão" <<endl;
    cout << "Assumindo que todas as variaveis >= 0" << endl << endl;
    cout << "Função Objetivo" <<endl;
    for(int i=0; i<num_var; i++){
        cout << "Entre com o coeficiente de X" << i+1 << ": ";
        double input;
        cin >> input;
        coef.push_back(input);
    }
    
    for(int j=0; j<num_rest; j++){
        cout << endl << "Restricao " << j+1;
        if(j==0)
            if(minimize)
                cout << "(Formato a1X1 + a2X2 + ... >= b)" << endl;
            else
                cout << "(Formato a1X1 + a2X2 + ... <= b)" << endl;
        else
            cout << endl;
        vector<double> tmp;
        for(int i=0; i<num_var; i++){
            cout << "Entre com o coeficiente de X" << i+1 << ": ";
            double input;
            cin >> input;
            tmp.push_back(input);
        }
        rest.push_back(tmp);
        cout << "Entre com o valor de b: ";
        double input;
        cin >> input;
        b.push_back(input);
    }
    cout << endl << endl;
            
    //coef = {3,4};
    //rest = {{1,1},{2,3}};
    //b = {9,18};
    
    //num_var = coef.size();
    //y
    num_rest = rest.size();
    
    if(minimize){               //Fazer o problema dual
        //troca coef e b
        vector<double> tmp = b;
        b = coef;
        coef = tmp;
        
                //Transpoe matriz restrições

        vector< vector<double> > rest_t(num_var,(vector<double>(num_rest)));
        for(int j=0; j<num_rest; j++)
            for(int i=0; i<num_var; i++)
                rest_t[i][j] = rest[j][i];
        rest = rest_t;
        
        int tmp2 = num_var;
        num_var = num_rest;
        num_rest = tmp2;
    }
    
    
    vector<double> theta(num_rest);
    
    //Forma canônica
    //Add slack variables
    for(int i=0; i<num_rest; i++){
        coef.push_back(0);
        for(int j= 0; j<num_rest; j++){
            if(i == j)
                rest[i].push_back(1);
            else
                rest[i].push_back(0);
        }
    }
    num_var_canon = coef.size();
    vector<double> c(num_var_canon);//linha cj-zj
    
    //Solução básica inicial
    //Guarda o número do índce de coef que representa a variável da base
    for(int i=num_var; i<num_var_canon; i++){
        base.push_back(i);
    }
    
    //Começa iteração   //j- linha; i - coluna
    while(1){
        iteration++;
               
        //Calcula cj-zj
        for(int i=0; i<num_var_canon; i++){
            double sum = 0;
            for(int j=0; j<num_rest; j++){
                sum += coef[base[j]]*rest[j][i]; //cj
            }
            c[i] = sum - coef[i];   //cj-zj
        }
        
        //Calcula z
        z = 0;
        for(int j=0; j<num_rest; j++)
             z += coef[base[j]]*b[j];
  
     
        //Deve parar (algum cj-zj negativo?)elements
        bool parar = true;
        for(int i=0; i<num_var_canon; i++)
            if(c[i] < 0)
                parar = false;
        if(parar){
            for(int i=0; i<theta.size(); i++)
                theta[i] = 0;//Para não imprimir o ultimo valor do theta, imprime 
            break;
        }
        
        //Pega coluna pivô
        int colp = 0;
        double menor = c[0];
        for(int i=1; i<num_var_canon; i++){
            if(c[i] < menor){
                menor = c[i];
                colp = i;
            }
        }
        
        //Calcula theta
        for(int j=0; j<num_rest; j++){
            theta[j] = b[j]/rest[j][colp];
        }
       
        
        //IMPRIME TABELA
        printTable(coef,rest,base,b,theta,c,z, iteration);

              
        //theta maior que zero --ARRUMAR!!!!
        parar = true;
        for(int j=0; j<num_rest; j++)
           if(theta[j] > 0)
               parar = false;
        if(parar)
            break;
        
        //Pega linha pivô --- while para caso o prmeiro item a comparar for menor que zero
        menor = -1;
        int linp = 0;
        while(menor < 0){
            int i = 0;
            menor = theta[i];
            i++;
        }
        for(int i=0; i<num_rest; i++){
            if(theta[i] < menor && theta[i] > 0){
                menor = theta[i];
                linp = i;
            }
        }
        
        //Para dizer se é degenerado
        bool segundo = false;
        for(int i=1; i<num_rest; i++){
            double teste = theta[i];
            double teste2 = fabs(teste - menor);
            if(teste2 < 0.00001){
                if(segundo){
                    cout << "Sistema Degenerado\n";
                    break;
                }
                segundo = true;
            }
        }
        
        //Elemento pivô
        pivo = rest[linp][colp];
         
        //Troca elemento da base
        base[linp] = colp;
        
        //Recalcula matriz
        for(int i=0; i<num_var_canon; i++){ //Linha do elemento pivo, dividir pelo elemento pivo
           double vv =  rest[linp][i];
            double a = rest[linp][i] / pivo;
            rest[linp][i] = rest[linp][i] / pivo;
        }
        
        vector<double> k(num_rest);
        for(int j=0; j<num_rest; j++){ //Para as outras linhas
            if(j != linp){ //Ignorar a linha do elemento pivo, ja calculada
                k[j] = rest[j][colp] / rest[linp][colp];     //Calcular cosntante k para multiplicar a linha
                for(int i=0; i<num_var_canon; i++)
                    rest[j][i] = rest[j][i] - (rest[linp][i] * k[j]);      //Recalcula as linhas 
            }
        }
        
        //Atualiza b
        b[linp] = b[linp] / pivo;
        for(int j=0; j<num_rest; j++){
            if(j != linp){
                b[j] = b[j] - (b[linp] * k[j]);
            }
        }
    }
    
    //Termino
    printTable(coef,rest,base,b,theta,c,z, iteration);
    
    cout << "Número de iterações: " << iteration << "\n";
    cout << "Z: " << z << "\n";
    
    if(minimize){
        //Pegar os valores das slack variables na última linha de cj-zj
        //Y1 corresponde a X1, Y2 corresponde a X2,....
        cout << "Minimização - Valores das variáveis do problema inicial aparecem em cj-zj das slack variables" << endl;
        for(int i=num_var ; i< num_var_canon; i++)
            cout << "X" << i-num_var << ": " << c[i] << endl;
        
    }
    
    else{
        //Imprimir apenas variáveis iniciais
        for(int i=0; i<num_var; i++){                           //Para cada variável básica
            double valor = 0;
            for(int j=0; j<base.size(); j++)                    //Para cada posição da base
                if(i == base[j])                                //Variável inicial está na base?
                    valor = b[j];                               //Se sim, imprime o valor que está em b para ela   
            cout << "X" << i+1 << ": " << valor<< "\n";      //Se não, coloca como zero    
        }
    }
    return 0;
}