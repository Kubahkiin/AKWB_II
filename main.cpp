#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <iterator>
#include <algorithm>
#include <map>
using namespace std;
//wysyłanie na git
void LoadGraph(vector< vector< int > >&Graph, const string& filename)
{
    cout << "Wczytywanie grafu z pliku: ";
    string line;
    int currentLine;

    ifstream graphFile ("graphs/"+ filename +".txt");

    if (graphFile.is_open())
    {
        currentLine = 0;
        while ( getline (graphFile,line) )
        {
            istringstream row(line); //przerobienie wczytanej linii na strumień inputów (tak jakby?)
            Graph.emplace_back(istream_iterator<int>(row), istream_iterator<int>()); // załadowanie całego wiersza
            currentLine++;
        }
        graphFile.close();
        cout << "Wczytano :)" << "\n";
    }
    else
        cout << "Nie mozna otworzyc pliku";
}
void SaveGraph(vector< vector< int > >&Graph, const string& filename)
{
    cout << "Zapisywanie grafu: ";
    ofstream graphFile ("graphs/"+ filename +"_out.txt");
    for (int i = 0; i < Graph.size(); i++)
    {
        for (int j = 0; j < Graph[i].size(); j++)
            graphFile << Graph[i][j] << " ";
        graphFile << "\n";
    }
    graphFile.close();
    cout << "Zapisano jako graphs/" << filename << "_out.txt" << "\n";
}
void DisplayGraph(vector< vector< int > >&Graph)
{
    cout << "Wyswietlanie wczytanego grafu:" << "\n";
    for (int i = 0; i < Graph.size(); i++)
    {
        cout << i << "-->";
        for (int j = 0; j < Graph[i].size(); j++)
            cout << Graph[i][j] << " ";
        cout << "\n";
    }
}
void DisplayArcs(map< int, pair<int, int> > &Arcs, int arcCount)
{
    cout << "Graf jako lista lukow" << "\n";
    for (int i = 0; i < arcCount; i++)
    {
        cout << Arcs[i].first << " " << Arcs[i].second << "\n";
    }
}

int CheckGraphType(vector< vector< int > >&Graph)
{
    int type;
    int commonVertices;
    bool linearity = true;


    for (int i = 0; i < Graph.size(); i++)
    {
        sort(Graph[i].begin(), Graph[i].end());

        for (int j = i+1; j < Graph.size(); j++)
        {

            for (int k = 0; k < Graph[i].size(); k++)
            {
                if ( k != Graph[i].size())
                {
                    if(Graph[i].size() > 1)
                    {
                        if ( Graph[i][k] == Graph[i][k+1] )
                        {
                            return 0;
                        }
                    }

                }
                commonVertices = 0;
                for (int l = 0; l < Graph[j].size(); l++)
                {
                    if ( Graph[i][k] == Graph[j][l] )
                    {
                        commonVertices++;
                        k++;
                    }
                    else if (commonVertices != 0)
                    {
                        return 0;
                    }
                }
                if ( commonVertices == 0 || commonVertices == Graph[i].size()  )
                {
                    if(commonVertices == Graph[i].size() && linearity)

                    {
                        bool iFound;
                        bool jFound;

                        for (int parent = 0; parent < Graph.size(); parent++)
                        {
                            iFound = false;
                            jFound = false;
                            for (int child = 0; child < Graph[parent].size(); child++)
                            {
                                if (Graph[parent][child] == i)
                                {
                                    iFound = true;
                                }
                                if (Graph[parent][child] == j)
                                {
                                    jFound = true;
                                }
                            }
                            if (iFound && jFound)
                                linearity = false;
                        }
                    }
                }


                else
                    return  0;
            }
        }
    }

    if (linearity)
        type = 2;
    else
        type = 1;

    return type;
}

void TransformIntoOriginal(vector< vector< int > >&Graph, vector< vector< int > >&OriginalGraph)
{

    map< int, pair<int, int> > Arcs;
    int arcCount = 0;
    for (int i = 0; i < Graph.size(); i++)
    {


            for (int j = 0; j < Graph[i].size(); j++)
            {
                Arcs[arcCount].first = i;
                Arcs[arcCount].second = Graph[i][j];
                arcCount++;
            }

    }



    map< int, pair<int, int> > OriginalArcs;
    int x = 0;
    for (int i = 0; i < Graph.size(); i++)
    {
        OriginalArcs[i].first = x++;
        OriginalArcs[i].second = x++;
    }



    int currentVertex;
    int buffedVertex;
    for (int i = 0; i < arcCount; i++)
    {
        currentVertex = OriginalArcs[ Arcs[i].first ].second;
        buffedVertex = OriginalArcs[ Arcs[i].second ].first;
        for(int j = 0; j < Graph.size(); j++)
        {
            if (OriginalArcs[j].first == buffedVertex)
                OriginalArcs[j].first = currentVertex;
            if (OriginalArcs[j].second == buffedVertex)
                OriginalArcs[j].second = currentVertex;

        }
    }
    cout << "Po transformacji" << "\n";


    int vertexToChange;
    for (int i = 0; i < Graph.size(); i++)
    {
        int miniValue = 1000;
        for (int  j= 0; j < Graph.size(); j++)
        {
            if (OriginalArcs[j].first < miniValue && OriginalArcs[j].first > i)
            {
                miniValue = OriginalArcs[j].first;
                vertexToChange = OriginalArcs[j].first;
            }
            if (OriginalArcs[j].second < miniValue && OriginalArcs[j].second > i)
            {
                miniValue = OriginalArcs[j].second;
                vertexToChange = OriginalArcs[j].second;
            }
        }
        for(int j = 0; j < Graph.size(); j++)
        {
            if (OriginalArcs[j].first == vertexToChange)
                OriginalArcs[j].first = i;
            if (OriginalArcs[j].second == vertexToChange)
                OriginalArcs[j].second = i;
        }
    }
    cout << "Po przeindeksowaniu" << "\n";

    int vertexCount = 0;
    for (int  i= 0; i < Graph.size(); i++)
    {
        if (OriginalArcs[i].first > vertexCount)
            vertexCount = OriginalArcs[i].first;
        if (OriginalArcs[i].second > vertexCount)
            vertexCount = OriginalArcs[i].second;
    }
    vertexCount++;



    OriginalGraph.resize(vertexCount);
    for (int i = 0; i < Graph.size(); i++)
    {
        OriginalGraph[ OriginalArcs[i].first ].push_back(OriginalArcs[i].second);
    }
}

bool GraphType(int type)
{
    switch (type)
    {
        case 0:
            cout << "Graf niesprzezony" << "\n";
            return false;
        case 1:
            cout << "Graf sprzezony" << "\n";
            return true;
        case 2:
            cout << "Graf liniowy" << "\n";
            return true;
        default:
            cout << "Sprawdzenie nie zadzialalo :(" << "\n";
            return false;
    }

}

int main()
{
    vector< vector< int > > Graph;
    vector< vector< int > > OriginalGraph;
    LoadGraph(Graph, "test2k23");

    if( GraphType( CheckGraphType(Graph) ) )
    {
        cout << "Posortowany" << "\n";
        DisplayGraph(Graph);

        TransformIntoOriginal(Graph, OriginalGraph);
        DisplayGraph(OriginalGraph);

        SaveGraph(OriginalGraph, "wynik");
    }
    return 0;
}