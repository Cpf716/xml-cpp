//
//  main.cpp
//  xml
//
//  Created by Corey Ferguson on 9/29/26.
//

#include "xml.h"
#include <fstream>
#include <sstream>

using namespace std;

int main(int argc, const char *argv[])
{
    fstream file("/Users/YOUR_USER/xml/xml/books.xml");

    if (!file.is_open())
        return EXIT_FAILURE;

    ostringstream xml;

    xml << file.rdbuf();

    unique_ptr<xml::element> xml_ptr(xml::parse(xml.str()));

    cout << xml_ptr.get()->str() << endl;
}
