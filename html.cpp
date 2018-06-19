#include "html.h"
#include <fstream>
#include <myhtml/api.h>
#include <boost/algorithm/string.hpp>


struct res_html {
    char  *html;
    size_t size;
};

bool load_html_file(const char* filename, res_html &res);
void print_node_attr(myhtml_tree_node_t *node);
void read_text(myhtml_tree_t* tree, myhtml_tree_node_t *node, std::string &out);
bool HtmlToText(res_html &res, std::string &out);

 bool ReadHtmlToText(const std::string &file, std::string &out)
{
    struct res_html res;

    if (load_html_file(file.c_str(), res)==false) return false;

    return HtmlToText(res, out);
}

 bool HtmlToText(res_html &res, std::string &out) {
    // basic init
    myhtml_t* myhtml = myhtml_create();
    myhtml_init(myhtml, MyHTML_OPTIONS_DEFAULT, 1, 0);

    // init tree
    myhtml_tree_t* tree = myhtml_tree_create();
    myhtml_tree_init(tree, myhtml);

    // parse html
    myhtml_parse(tree, MyENCODING_UTF_8, res.html, res.size);

    // print tree
    myhtml_tree_node_t *node = myhtml_tree_get_document(tree);
    read_text(tree, myhtml_node_child(node), out);

    // release resources
    myhtml_tree_destroy(tree);
    myhtml_destroy(myhtml);

    free(res.html);

    return true;
}

bool load_html_file(const char* filename, res_html &res)
{
    FILE *fh = fopen(filename, "rb");
    if(fh == NULL) {
        fprintf(stderr, "Can't open html file: %s\n", filename);
        return false;
    }

    if(fseek(fh, 0L, SEEK_END) != 0) {
        fprintf(stderr, "Can't set position (fseek) in file: %s\n", filename);
        return false;
    }

    long size = ftell(fh);

    if(fseek(fh, 0L, SEEK_SET) != 0) {
        fprintf(stderr, "Can't set position (fseek) in file: %s\n", filename);
        return false;
    }

    if(size <= 0) {
        fprintf(stderr, "Can't get file size or file is empty: %s\n", filename);
        return false;
    }

    char *html = (char*)malloc(size + 1);
    if(html == NULL) {
        fprintf(stderr, "Can't allocate mem for html file: %s\n", filename);
        return false;
    }

    size_t nread = fread(html, 1, size, fh);
    if (nread != (size_t) size) {
        fprintf(stderr, "could not read %ld bytes (%ld bytes done)\n", size, (long)nread);
        return false;
    }

    fclose(fh);

    res = {html, (size_t)size};

    return true;
}

void print_node_attr(myhtml_tree_node_t *node)
{
    myhtml_tree_attr_t *attr = myhtml_node_attribute_first(node);

    while (attr) {
        const char *name = myhtml_attribute_key(attr, NULL);

        if(name) {
            printf(" %s", name);

            const char *value = myhtml_attribute_value(attr, NULL);

            if(value)
                printf("=\"%s\"", value);
        }

        attr = myhtml_attribute_next(attr);
    }
}

void read_text(myhtml_tree_t* tree, myhtml_tree_node_t *node, std::string &out)
{
    while (node)
    {
        myhtml_tag_id_t tag_id = myhtml_node_tag_id(node);

        if(tag_id == MyHTML_TAG__TEXT) {
            const char* node_text = myhtml_node_text(node, NULL);
            std::string tmp = node_text;
            boost::trim(tmp);
            if(tmp.length()>0)
                out+=tmp+" ";
        }

        if(tag_id != MyHTML_TAG_STYLE && tag_id != MyHTML_TAG_SCRIPT)
            read_text(tree, myhtml_node_child(node), out);
        node = myhtml_node_next(node);
    }
}

bool HtmlToText(const std::string &html, std::string &out)
{
    struct res_html res;

    res.size = html.length();
    res.html = (char*)malloc(res.size + 1);

    memcpy(res.html, html.data(), res.size);

    return HtmlToText(res, out);
}
