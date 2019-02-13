
char* to_char(string s)
{
    return (char*)(s.c_str());
}

TTree* Read_Tree(TFile* File)
{
    TTree * tree = (TTree*)File -> Get("mppc;1");
    return tree;
}

TBranch * Get_Event_Branch(TTree * Tree, short *c)
{
    auto branch = Tree -> GetBranch("chg");
    branch -> SetAddress(c);
    return branch;
}

void read(string Root_File_Prefix)
{
    string save_file_name = Root_File_Prefix + "_Histo.root";
    string root_file_name = Root_File_Prefix + ".root";

    auto f1 = new TFile(to_char(root_file_name));
    auto tree1 = Read_Tree(f1);
    short chg1[32];
    auto branch1 = Get_Event_Branch(tree1, chg1);


    branch1 -> GetEntry(1);

    // for(int i = 0; i < 32; i++)
    // {
    //     cout << " 1 " << chg1[i] << endl;
    //     cout << chg2[i] << endl;
    // }


    int N1 = tree1 -> GetEntries();
    TH1I *hg1[32];
    for(int i = 0; i < 32; i++)
    {
        string s_ch = (string)"ch_" + to_string(i);
        auto h1 = new TH1I(to_char(((string)"Ch")+s_ch), to_char(((string)"Ch")+s_ch), 4096, 0, 4096);
        hg1[i] = h1;

    }
    for(int event = 0; event < N1; event++)
    {
        branch1 -> GetEntry(event);
        for(int ch = 0; ch < 32; ch++)
        {
            hg1[ch] -> Fill(chg1[ch]);
        }
    }



    auto save_file = new TFile(to_char(save_file_name), "recreate");
    for(int i = 0; i < 32; i++)
    {
        hg1[i] -> Write();
    }



    save_file -> Close();
    // f1 -> Close();
    // f2 -> Close();



    
    
}