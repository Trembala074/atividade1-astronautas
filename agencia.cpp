#include <iostream>
#include <string>
#include <vector>
using namespace std;

// ===================== CLASSE ASTRONAUTA =====================
struct Astronauta {
    string cpf;
    string nome;
    int idade;
    bool vivo;
    bool disponivel;
    vector<int> voosParticipados; // códigos dos voos que foram lançados

    Astronauta(string c, int id, string n) {
        cpf = c;
        idade = id;
        nome = n;
        vivo = true;
        disponivel = true;
    }
};

// ===================== CLASSE VOO =====================
// Estados: 0 = planejado, 1 = em curso, 2 = finalizado com sucesso, 3 = finalizado com explosão
struct Voo {
    int codigo;
    vector<string> cpfsPassageiros;
    int estado;

    Voo(int cod) {
        codigo = cod;
        estado = 0;
    }

    string estadoStr() const {
        if (estado == 0) return "planejado";
        if (estado == 1) return "em curso";
        if (estado == 2) return "finalizado com sucesso";
        return "finalizado com explosao";
    }
};

// ===================== DADOS GLOBAIS =====================
vector<Astronauta> astronautas;
vector<Voo> voos;

// ===================== FUNÇÕES AUXILIARES =====================
int findAstronauta(const string& cpf) {
    for (int i = 0; i < (int)astronautas.size(); i++)
        if (astronautas[i].cpf == cpf) return i;
    return -1;
}

int findVoo(int codigo) {
    for (int i = 0; i < (int)voos.size(); i++)
        if (voos[i].codigo == codigo) return i;
    return -1;
}

bool astronautaNoVoo(const Voo& v, const string& cpf) {
    for (int i = 0; i < (int)v.cpfsPassageiros.size(); i++)
        if (v.cpfsPassageiros[i] == cpf) return true;
    return false;
}

// ===================== OPERAÇÕES =====================

void cadastrarAstronauta(const string& cpf, int idade, const string& nome) {
    if (findAstronauta(cpf) != -1) {
        cout << "Erro: ja existe um astronauta com o CPF " << cpf << "." << endl;
        return;
    }
    astronautas.push_back(Astronauta(cpf, idade, nome));
    cout << "Astronauta " << nome << " cadastrado com sucesso." << endl;
}

void cadastrarVoo(int codigo) {
    if (findVoo(codigo) != -1) {
        cout << "Erro: ja existe um voo com o codigo " << codigo << "." << endl;
        return;
    }
    voos.push_back(Voo(codigo));
    cout << "Voo " << codigo << " cadastrado com sucesso." << endl;
}

void adicionarAstronauta(const string& cpf, int codigo) {
    int ia = findAstronauta(cpf);
    if (ia == -1) {
        cout << "Erro: astronauta com CPF " << cpf << " nao encontrado." << endl;
        return;
    }
    int iv = findVoo(codigo);
    if (iv == -1) {
        cout << "Erro: voo " << codigo << " nao encontrado." << endl;
        return;
    }
    if (voos[iv].estado != 0) {
        cout << "Erro: o voo " << codigo << " nao esta planejado." << endl;
        return;
    }
    if (!astronautas[ia].vivo) {
        cout << "Erro: o astronauta " << astronautas[ia].nome << " esta morto." << endl;
        return;
    }
    if (astronautaNoVoo(voos[iv], cpf)) {
        cout << "Erro: o astronauta " << astronautas[ia].nome << " ja esta no voo " << codigo << "." << endl;
        return;
    }
    voos[iv].cpfsPassageiros.push_back(cpf);
    cout << "Astronauta " << astronautas[ia].nome << " adicionado ao voo " << codigo << "." << endl;
}

void removerAstronauta(const string& cpf, int codigo) {
    int ia = findAstronauta(cpf);
    if (ia == -1) {
        cout << "Erro: astronauta com CPF " << cpf << " nao encontrado." << endl;
        return;
    }
    int iv = findVoo(codigo);
    if (iv == -1) {
        cout << "Erro: voo " << codigo << " nao encontrado." << endl;
        return;
    }
    if (voos[iv].estado != 0) {
        cout << "Erro: o voo " << codigo << " nao esta planejado." << endl;
        return;
    }
    // Procura e remove
    vector<string>& lista = voos[iv].cpfsPassageiros;
    for (int i = 0; i < (int)lista.size(); i++) {
        if (lista[i] == cpf) {
            lista.erase(lista.begin() + i);
            cout << "Astronauta " << astronautas[ia].nome << " removido do voo " << codigo << "." << endl;
            return;
        }
    }
    cout << "Erro: o astronauta " << astronautas[ia].nome << " nao esta no voo " << codigo << "." << endl;
}

void lancarVoo(int codigo) {
    int iv = findVoo(codigo);
    if (iv == -1) {
        cout << "Erro: voo " << codigo << " nao encontrado." << endl;
        return;
    }
    if (voos[iv].estado != 0) {
        cout << "Erro: o voo " << codigo << " nao esta planejado." << endl;
        return;
    }
    if (voos[iv].cpfsPassageiros.empty()) {
        cout << "Erro: o voo " << codigo << " nao tem astronautas." << endl;
        return;
    }
    // Verifica todos os astronautas
    for (int i = 0; i < (int)voos[iv].cpfsPassageiros.size(); i++) {
        string cpf = voos[iv].cpfsPassageiros[i];
        int ia = findAstronauta(cpf);
        if (ia == -1) {
            cout << "Erro: astronauta com CPF " << cpf << " nao encontrado." << endl;
            return;
        }
        if (!astronautas[ia].vivo) {
            cout << "Erro: o astronauta " << astronautas[ia].nome << " esta morto." << endl;
            return;
        }
        if (!astronautas[ia].disponivel) {
            cout << "Erro: o astronauta " << astronautas[ia].nome << " nao esta disponivel." << endl;
            return;
        }
    }
    // Lança
    voos[iv].estado = 1;
    for (int i = 0; i < (int)voos[iv].cpfsPassageiros.size(); i++) {
        string cpf = voos[iv].cpfsPassageiros[i];
        int ia = findAstronauta(cpf);
        astronautas[ia].disponivel = false;
        astronautas[ia].voosParticipados.push_back(codigo);
    }
    cout << "Voo " << codigo << " lancado com sucesso." << endl;
}

void explodirVoo(int codigo) {
    int iv = findVoo(codigo);
    if (iv == -1) {
        cout << "Erro: voo " << codigo << " nao encontrado." << endl;
        return;
    }
    if (voos[iv].estado != 1) {
        cout << "Erro: apenas voos em curso podem explodir." << endl;
        return;
    }
    voos[iv].estado = 3;
    for (int i = 0; i < (int)voos[iv].cpfsPassageiros.size(); i++) {
        string cpf = voos[iv].cpfsPassageiros[i];
        int ia = findAstronauta(cpf);
        if (ia != -1) {
            astronautas[ia].vivo = false;
            astronautas[ia].disponivel = false;
        }
    }
    cout << "Voo " << codigo << " explodiu. Todos os astronautas morreram." << endl;
}

void finalizarVoo(int codigo) {
    int iv = findVoo(codigo);
    if (iv == -1) {
        cout << "Erro: voo " << codigo << " nao encontrado." << endl;
        return;
    }
    if (voos[iv].estado != 1) {
        cout << "Erro: apenas voos em curso podem ser finalizados com sucesso." << endl;
        return;
    }
    voos[iv].estado = 2;
    for (int i = 0; i < (int)voos[iv].cpfsPassageiros.size(); i++) {
        string cpf = voos[iv].cpfsPassageiros[i];
        int ia = findAstronauta(cpf);
        if (ia != -1) {
            astronautas[ia].disponivel = true;
        }
    }
    cout << "Voo " << codigo << " finalizado com sucesso." << endl;
}

void listarVoos() {
    // Grupos: 0=planejado, 1=em curso, 2=finalizado com sucesso, 3=finalizado com explosão
    int grupos[4] = {0, 1, 2, 3};
    string nomeGrupo[4] = {"Planejados", "Em curso", "Finalizados com sucesso", "Finalizados com explosao"};

    cout << "=== Lista de Voos ===" << endl;
    for (int g = 0; g < 4; g++) {
        cout << nomeGrupo[g] << ":" << endl;
        bool algum = false;
        for (int i = 0; i < (int)voos.size(); i++) {
            if (voos[i].estado == grupos[g]) {
                algum = true;
                cout << "  Voo " << voos[i].codigo << " [" << voos[i].estadoStr() << "]" << endl;
                if (voos[i].cpfsPassageiros.empty()) {
                    cout << "    Sem astronautas." << endl;
                } else {
                    for (int j = 0; j < (int)voos[i].cpfsPassageiros.size(); j++) {
                        string cpf = voos[i].cpfsPassageiros[j];
                        int ia = findAstronauta(cpf);
                        if (ia != -1)
                            cout << "    CPF: " << cpf << " | Nome: " << astronautas[ia].nome << endl;
                        else
                            cout << "    CPF: " << cpf << endl;
                    }
                }
            }
        }
        if (!algum) cout << "  Nenhum." << endl;
    }
}

void listarMortos() {
    cout << "=== Astronautas Mortos ===" << endl;
    bool algum = false;
    for (int i = 0; i < (int)astronautas.size(); i++) {
        if (!astronautas[i].vivo) {
            algum = true;
            cout << "CPF: " << astronautas[i].cpf << " | Nome: " << astronautas[i].nome << endl;
            if (astronautas[i].voosParticipados.empty()) {
                cout << "  Voos: nenhum lancado." << endl;
            } else {
                cout << "  Voos participados: ";
                for (int j = 0; j < (int)astronautas[i].voosParticipados.size(); j++) {
                    if (j > 0) cout << ", ";
                    cout << astronautas[i].voosParticipados[j];
                }
                cout << endl;
            }
        }
    }
    if (!algum) cout << "Nenhum astronauta morto." << endl;
}

// ===================== MAIN =====================
int main() {
    string comando;
    while (cin >> comando) {
        if (comando == "FIM") {
            break;
        }
        else if (comando == "CADASTRAR_ASTRONAUTA") {
            string cpf, nome;
            int idade;
            cin >> cpf >> idade;
            getline(cin >> ws, nome);
            cadastrarAstronauta(cpf, idade, nome);
        }
        else if (comando == "CADASTRAR_VOO") {
            int codigo;
            cin >> codigo;
            cadastrarVoo(codigo);
        }
        else if (comando == "ADICIONAR_ASTRONAUTA") {
            string cpf;
            int codigo;
            cin >> cpf >> codigo;
            adicionarAstronauta(cpf, codigo);
        }
        else if (comando == "REMOVER_ASTRONAUTA") {
            string cpf;
            int codigo;
            cin >> cpf >> codigo;
            removerAstronauta(cpf, codigo);
        }
        else if (comando == "LANCAR_VOO") {
            int codigo;
            cin >> codigo;
            lancarVoo(codigo);
        }
        else if (comando == "EXPLODIR_VOO") {
            int codigo;
            cin >> codigo;
            explodirVoo(codigo);
        }
        else if (comando == "FINALIZAR_VOO") {
            int codigo;
            cin >> codigo;
            finalizarVoo(codigo);
        }
        else if (comando == "LISTAR_VOOS") {
            listarVoos();
        }
        else if (comando == "LISTAR_MORTOS") {
            listarMortos();
        }
        else {
            cout << "Erro: comando desconhecido: " << comando << endl;
        }
    }
    return 0;
}
