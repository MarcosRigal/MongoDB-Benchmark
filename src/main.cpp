#include <chrono>

#include <bsoncxx/builder/basic/array.hpp>
#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/builder/basic/kvp.hpp>
#include <bsoncxx/types.hpp>

#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/uri.hpp>

using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_array;
using bsoncxx::builder::basic::make_document;

int main(int, char **)
{
    mongocxx::instance inst{};
    mongocxx::client conn{mongocxx::uri{}};

    auto db = conn["test"];

    bsoncxx::document::value restaurant_doc = make_document(
        kvp("address",
            make_document(kvp("street", "2 Avenue"),
                          kvp("zipcode", 10075),
                          kvp("building", "1480"),
                          kvp("coord", make_array(-73.9557413, 40.7720266)))),
        kvp("borough", "Manhattan"),
        kvp("cuisine", "Italian"),
        kvp("grades",
            make_array(
                make_document(kvp("date", bsoncxx::types::b_date{std::chrono::milliseconds{12323}}),
                              kvp("grade", "A"),
                              kvp("score", 11)),
                make_document(
                    kvp("date", bsoncxx::types::b_date{std::chrono::milliseconds{121212}}),
                    kvp("grade", "B"),
                    kvp("score", 17)))),
        kvp("name", "Vella"),
        kvp("restaurant_id", "41704620"));

    auto res = db["restaurants"].insert_one(std::move(restaurant_doc));
}