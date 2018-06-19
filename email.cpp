#include "email.h"
#include "mimetic/mimetic.h"
#include <boost/algorithm/string.hpp>

using namespace mimetic;

void getEmailMessage(MimeEntity *entity, std::string &out);
void miltipartEmail(MimeEntity *entity, std::string &out);

 bool ReadEmail(const std::string &file, std::map<std::string, std::string> &out)
{
    out.clear();

    std::ifstream is (file, std::ifstream::binary);
    MimeEntity me;
    me.load(is);
    is.close();

    std::string senders = me.header().sender().str();
    senders += ";";
    senders += me.header().from().str();
    boost::trim(senders);
    out["Senders"] = senders;

    std::string subject = me.header().subject();
    boost::trim(subject);
    out["Subject"] = subject;

    out["Text"] = "";
    out["Text"].reserve(me.size());
    getEmailMessage(&me, out["Text"]);
    boost::trim(out["Text"]);

    return true;
}

void getEmailMessage(MimeEntity *entity, std::string &out) {
    if (entity->header().contentType().isMultipart()) {
        miltipartEmail(entity, out);
    } else {
        if (entity->header().contentType().type().compare(0,4,"text") == 0) {
            out += entity->body();
        }
    }
}

void miltipartEmail(MimeEntity *entity, std::string &out) {
    for (auto &s : entity->body().parts()) {
        if (s->header().contentType().isMultipart()){
            miltipartEmail(s, out);
        } else {
            if (s->header().contentType().type().compare(0,4,"text") != 0) {
                continue;
            }
            out += s->body();
        }
    }
}
