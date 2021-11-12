#pragma once

#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <utility>
#include <fstream>
#include <cstdlib>
#include <json11.hpp>
#include <cmath>

using namespace std;
using namespace json11;

const vector<string> Names = {"OLIVE", "JAC", "HARRY", "JACOB", "CHARLIE", "THOMAS", "GEORGE", "OSCAR", "JAMES", "WILLIAM", "NOAH", "ALFIE", "JOSHUA",
                              "MUHAMMAD", "HENRY", "LEO", "ARCHIE", "ETHAN", "JOSEPH", "FREDDIE", "SAMUEL", "ALEXANDER", "LOGAN", "DANIEL", "ISAAC", "MAX",
                              "BENJAMIN", "MASON", "LUCAS", "EDWARD", "HARRISON", "JAKE", "DYLAN", "RILEY", "FINLEY", "THEO", "SEBASTIAN", "ADAM", "ZACHARY",
                              "ARTHUR", "TOBY", "JAYDEN", "LUKE", "HARLEY", "LEWIS", "TYLER", "HARVEY", "MATTHEW", "DAVID", "REUBEN", "MICHAEL", "ELIJAH",
                              "KIAN", "TOMMY", "MOHAMMAD", "BLAKE", "LUCA", "THEODORE", "STANLEY", "JENSON", "NATHAN", "CHARLES", "FRANKIE", "JUDE", "TEDDY",
                              "LOUIE", "LOUIS", "RYAN", "HUGO", "BOBBY", "ELLIOTT", "DEXTER", "OLLIE", "ALEX", "LIAM", "KAI", "GABRIEL", "CONNOR", "AARON",
                              "FREDERICK", "CALLUM", "ELLIOT", "ALBERT", "LEON", "RONNIE", "RORY", "JAMIE", "AUSTIN", "SETH", "IBRAHIM", "OWEN", "CALEB",
                              "ELLIS", "SONNY", "ROBERT", "JOEY", "FELIX", "FINLAY", "JACKSON", "AMELIA", "OLIVIA", "ISLA", "EMILY", "POPPY", "AVA", "ISABELLA",
                              "JESSICA", "LILY", "SOPHIE", "GRACE", "SOPHIA", "MIA", "EVIE", "RUBY", "ELLA", "SCARLETT", "ISABELLE", "CHLOE", "SIENNA", "FREYA",
                              "PHOEBE", "CHARLOTTE", "DAISY", "ALICE", "FLORENCE", "EVA", "SOFIA", "MILLIE", "LUCY", "EVELYN", "ELSIE", "ROSIE", "IMOGEN",
                              "LOLA", "MATILDA", "ELIZABETH", "LAYLA", "HOLLY", "LILLY", "MOLLY", "ERIN", "ELLIE", "MAISIE", "MAYA", "ABIGAIL", "ELIZA",
                              "GEORGIA", "JASMINE", "ESME", "WILLOW", "BELLA", "ANNABELLE", "IVY", "AMBER", "EMILIA", "EMMA", "SUMMER", "HANNAH", "ELEANOR",
                              "HARRIET", "ROSE", "AMELIE", "LEXI", "MEGAN", "GRACIE", "ZARA", "LACEY", "MARTHA", "ANNA", "VIOLET", "DARCEY", "MARIA",
                              "MARYAM", "BROOKE", "AISHA", "KATIE", "LEAH", "THEA", "DARCIE", "HOLLIE", "AMY", "MOLLIE", "HEIDI", "LOTTIE", "BETHANY",
                              "FRANCESCA", "FAITH", "HARPER", "NANCY", "BEATRICE", "ISABEL", "DARCY", "LYDIA", "SARAH",
                              "SARA", "ROSLOVCEV"};

const vector<string> Countries = {"Afghanistan", "Albania", "Algeria", "American Samoa", "Andorra", "Angola", "Anguilla", "Antarctica", "Antigua And Barbuda",
                                  "Argentina", "Armenia", "Aruba", "Australia", "Austria", "Azerbaijan", "Bahamas", "Bahrain", "Bangladesh", "Barbados",
                                  "Belarus", "Belgium", "Belize", "Benin", "Bermuda", "Bhutan", "Bolivia", "Bosnia And Herzegovina", "Botswana",
                                  "Bouvet Island", "Brazil", "British Indian Ocean Territory", "Brunei Darussalam", "Bulgaria", "Burkina Faso", "Burundi",
                                  "Cambodia", "Cameroon", "Canada", "Cape Verde", "Cayman Islands", "Central African Republic", "Chad", "Chile", "China",
                                  "Christmas Island", "Cocos (keeling) Islands", "Colombia", "Comoros", "Congo", "Congo, The Democratic Republic Of The",
                                  "Cook Islands", "Costa Rica", "Cote D'ivoire", "Croatia", "Cuba", "Cyprus", "Czech Republic", "Denmark", "Djibouti",
                                  "Dominica", "Dominican Republic", "East Timor", "Ecuador", "Egypt", "El Salvador", "Equatorial Guinea", "Eritrea", "Estonia",
                                  "Ethiopia", "Falkland Islands", "Faroe Islands", "Fiji", "Finland", "France", "French Guiana", "French Polynesia",
                                  "French Southern Territories", "Gabon", "Gambia", "Georgia", "Germany", "Ghana", "Gibraltar", "Greece", "Greenland",
                                  "Grenada", "Guadeloupe", "Guam", "Guatemala", "Guinea", "Guinea-bissau", "Guyana", "Haiti",
                                  "Heard Island And Mcdonald Islands", "Holy See (vatican City State)", "Honduras", "Hong Kong", "Hungary", "Iceland", "India",
                                  "Indonesia", "Iran", "Iraq", "Ireland", "Israel", "Italy", "Jamaica", "Japan", "Jordan", "Kazakstan",
                                  "Kenya", "Kiribati", "Korea, Democratic People's Republic Of", "Korea, Republic Of", "Kosovo", "Kuwait", "Kyrgyzstan",
                                  "Lao People's Democratic Republic", "Latvia", "Lebanon", "Lesotho", "Liberia", "Libyan Arab Jamahiriya", "Liechtenstein",
                                  "Lithuania", "Luxembourg", "Macau", "Macedonia, The Former Yugoslav Republic Of", "Madagascar", "Malawi", "Malaysia",
                                  "Maldives", "Mali", "Malta", "Marshall Islands", "Martinique", "Mauritania", "Mauritius", "Mayotte", "Mexico",
                                  "Micronesia, Federated States Of", "Moldova, Republic Of", "Monaco", "Mongolia", "Montserrat", "Montenegro", "Morocco",
                                  "Mozambique", "Myanmar", "Namibia", "Nauru", "Nepal", "Netherlands", "Netherlands Antilles", "New Caledonia", "New Zealand",
                                  "Nicaragua", "Niger", "Nigeria", "Niue", "Norfolk Island", "Northern Mariana Islands", "Norway", "Oman", "Pakistan", "Palau",
                                  "Palestinian Territory, Occupied", "Panama", "Papua New Guinea", "Paraguay", "Peru", "Philippines", "Pitcairn", "Poland",
                                  "Portugal", "Puerto Rico", "Qatar", "Reunion", "Romania", "Russian Federation", "Rwanda", "Saint Helena",
                                  "Saint Kitts And Nevis", "Saint Lucia", "Saint Pierre And Miquelon", "Saint Vincent And The Grenadines", "Samoa",
                                  "San Marino", "Sao Tome And Principe", "Saudi Arabia", "Senegal", "Serbia", "Seychelles", "Sierra Leone", "Singapore",
                                  "Slovakia", "Slovenia", "Solomon Islands", "Somalia", "South Africa", "South Georgia And The South Sandwich Islands", "Spain",
                                  "Sri Lanka", "Sudan", "Suriname", "Svalbard And Jan Mayen", "Swaziland", "Sweden", "Switzerland", "Syrian Arab Republic",
                                  "Taiwan, Province Of China", "Tajikistan", "Tanzania, United Republic Of", "Thailand", "Togo", "Tokelau", "Tonga",
                                  "Trinidad And Tobago", "Tunisia", "Turkey", "Turkmenistan", "Turks And Caicos Islands", "Tuvalu", "Uganda", "Ukraine",
                                  "United Arab Emirates", "United Kingdom", "United States", "United States Minor Outlying Islands", "Uruguay", "Uzbekistan",
                                  "Vanuatu", "Venezuela", "Viet Nam", "Virgin Islands, British", "Virgin Islands, U.s.", "Wallis And Futuna", "Western Sahara",
                                  "Yemen", "Zambia", "Zimbabwe"};

struct Man
{
    bool married;
    int age;
    double weight;
    double height;
    string name;
    string country;

    json11::Json to_json() const
    {
        return json11::Json::object({
                                            {"Name",    name},
                                            {"Married", married},
                                            {"Age",     age},
                                            {"Weight",  weight},
                                            {"Height",  height},
                                            {"Country", country},
                                    });
    }

    inline string getStringForHash() const
    {
        return name + to_string(age);
    }

    auto operator<=>(const Man &) const = default;
};

inline std::ostream &operator<<(std::ostream &os, const Man &man)
{
    os << "{\n\"Name\": \"" << man.name << "\",\n";
    if (man.married)
        os << "\"Married\": true,\n";
    else
        os << "\"Married\": false,\n";
    os << "\"Age\": " << man.age << ",\n";
    os << "\"Weight\": " << man.weight << ",\n";
    os << "\"Height\": " << man.height << ",\n";
    os << "\"Country\": \"" << man.country << "\"\n},\n";
    return os;
}

Man randomMan(string manName = " ", string manCountry = " ")
{
    if (manName == " ")
        manName = Names[rand() % Names.size()];
    if (manCountry == " ")
        manCountry = Countries[rand() % Countries.size()];

    Man man;
    man.married = rand() % 2;
    man.age = 10 + rand() % 50;
    man.height = 160 + rand() % 50;
    man.weight = ceil(0.35 * man.height) + rand() % 7;
    man.name = manName;
    man.country = manCountry;

    return man;
}

// Приоритет от 0 до 6
struct PriorityMan
{
    int name;
    int married;
    int age;
    int weight;
    int height;
    int country;

    PriorityMan(int name, int married, int age, int weight, int height, int country) : married(married), age(age), weight(weight), height(height),
                                                                                       name(name), country(country)
    {}
};
