#ifndef EMAIL_H
#define EMAIL_H

#include <string>
#include <map>


/*
 Wyjściowo mapa zawiera trzy elementy:
 *Senders
 *Subject
 *Text
 */
bool ReadEmail(const std::string &file, std::map<std::string, std::string> &out);

#endif // EMAIL_H
