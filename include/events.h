#include <cstdint>
#include "folly/AtomicUnorderedMap.h"
#include "folly/IPAddress.h"
#include "eventpp/eventdispatcher.h"

enum pal_event_basic_types : uint8_t {
    EVENT_PRE,
    EVENT_POST,
    EVENT_ASYNC,
    EVENT_DEBUG = 0xFF,
};

enum pal_event_subsystem_types : uint8_t {
    LOADER,
    ENGINE,
    GAME_USERAUTH,
    GAME_GAMEPLAY,
};

enum pal_event_types : uint16_t {
    LOGIN
};

#define BUILD_PAL_EVENT_ID(B, S, E) uint32_t((B) << 24 | (S) << 16 | (E))

enum pal_avalable_event_types : uint32_t {
    EVENT_PRE_LOGIN   = BUILD_PAL_EVENT_ID(EVENT_PRE, GAME_USERAUTH, LOGIN),
    EVENT_LOGIN_ASYNC = BUILD_PAL_EVENT_ID(EVENT_ASYNC, GAME_USERAUTH, LOGIN),
};

struct pal_loader_basic_event {
        const pal_event_basic_types     basic_type;
        const pal_event_subsystem_types subsystem_type;
        const pal_event_types           type;

        pal_loader_basic_event() = delete;

        pal_loader_basic_event(pal_event_basic_types b_type, pal_event_subsystem_types s_type, pal_event_types e_type)
            : basic_type(b_type), subsystem_type(s_type), type(e_type) {}
};

struct user_login_event_async : pal_loader_basic_event {
    private:
        folly::fbstring      user_name;
        std::vector<uint8_t> unique_net_id;
        folly::IPAddress     remote_address;

    public:
        user_login_event_async() = delete;

        user_login_event_async(const folly::fbstring user_name, const std::vector<uint8_t> unique_net_id, const folly::IPAddress remote_address)
            : pal_loader_basic_event(EVENT_ASYNC, GAME_USERAUTH, LOGIN),
              user_name(user_name),
              unique_net_id(unique_net_id),
              remote_address(remote_address) {}

        const folly::fbstring &get_user_name() {
            return user_name;
        };

        const std::vector<uint8_t> &get_unique_net_id() {
            return unique_net_id;
        };

        const folly::IPAddress &get_remote_address() {
            return remote_address;
        };
};

struct user_login_event : pal_loader_basic_event {
    private:
        folly::fbstring      user_name;
        std::vector<uint8_t> unique_net_id;
        folly::IPAddress     remote_address;

    public:
        user_login_event() = delete;

        user_login_event(const folly::fbstring &user_name, const std::vector<uint8_t> &unique_net_id, const folly::IPAddress &remote_address)
            : pal_loader_basic_event(EVENT_PRE, GAME_USERAUTH, LOGIN),
              user_name(user_name),
              unique_net_id(unique_net_id),
              remote_address(remote_address) {}

        const folly::fbstring &get_user_name() {
            return user_name;
        };

        const std::vector<uint8_t> &get_unique_net_id() {
            return unique_net_id;
        };

        const folly::IPAddress &get_remote_address() {
            return remote_address;
        };
};

struct pal_loader_event_policies {
        static uint32_t getEvent(const pal_loader_basic_event &e) {
            return e.basic_type << 24 | e.subsystem_type << 16 | e.type;
        }
};

extern eventpp::EventDispatcher<uint32_t, bool(const pal_loader_basic_event &e), pal_loader_event_policies> dispatcher;