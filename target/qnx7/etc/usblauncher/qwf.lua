--
-- QWF supported modem modules/dongles
--
-- Telit LE910 NA V2 modem module (1 CDC-NCM + 3 CDC-ACM) (Intel XMM7160)
-- The modemId should be the model series name (le910v2)
device(0x1bc7, 0x0036) {
    interface(0) {
--      driver"/scripts/service-cellular.sh start qwf-crm-telit-le910 le910v2 ncm0 $(vendor_id) $(product_id) $(busno) $(devno)";
        start"/scripts/service-cellular.sh start qwf-crm-telit-le910 le910v2 ncm0 $(vendor_id) $(product_id) $(busno) $(devno)";
        removal"/scripts/service-cellular.sh stop";
    };
    -- Don't try matching any rules for the other interfaces
    interface(1,127) {
        Ignore;
    };
};

-- Telit LE910C4 (Qualcomm) (DIAG + ADB + ECM + NMEA + MODEM + MODEM + SAP)
-- ECM - /dev/usbcellular1
--  AT - /dev/usbcellular2
--  AT - /dev/usbcellular3
device(0x1bc7, 0x1206) {
    interface(0) {
--      driver"/scripts/service-cellular.sh start qwf-crm-telit-le910 le910c4 ecm0 $(vendor_id) $(product_id) $(busno) $(devno)";
        start"/scripts/service-cellular.sh start qwf-crm-telit-le910 le910c4 ecm0 $(vendor_id) $(product_id) $(busno) $(devno)";
        removal"/scripts/service-cellular.sh stop";
    };
    -- Don't try matching any rules for the other interfaces
    interface(1,127) {
        Ignore;
    };
};

-- Telit LE910/LE920 modem modules
device(0x1bc7, 0x1201) {
    interface(0) {
--      driver"/scripts/service-cellular.sh start qwf-crm-telit le910 ppp0 $(vendor_id) $(product_id) $(busno) $(devno)";
        start"/scripts/service-cellular.sh start qwf-crm-telit le910 ppp0 $(vendor_id) $(product_id) $(busno) $(devno)";
        removal"/scripts/service-cellular.sh stop";
    };
    -- Don't try matching any rules for the other interfaces
    interface(1,127) {
        Ignore;
    };
};

-- Telit LE940B6 modem module (1 CDC-NCM + 3 CDC-ACM) (Intel)
device(0x1bc7, 0x0100) {
    interface(0) {
--      driver"/scripts/service-cellular.sh start qwf-crm-telit-le940b6 le940b6 ncm0 $(vendor_id) $(product_id) $(busno) $(devno)";
        start"/scripts/service-cellular.sh start qwf-crm-telit-le940b6 le940b6 ncm0 $(vendor_id) $(product_id) $(busno) $(devno)";
        removal"/scripts/service-cellular.sh stop";
    };
    -- Don't try matching any rules for the other interfaces
    interface(1,127) {
        Ignore;
    };
};

-- MultiTech MTD-H5 modem module (Telit HE910)
device(0x1bc7, 0x0021) {
    interface(0) {
--      driver"/scripts/service-cellular.sh start qwf-crm-telit he910 ppp0 $(vendor_id) $(product_id) $(busno) $(devno)";
        start"/scripts/service-cellular.sh start qwf-crm-telit he910 ppp0 $(vendor_id) $(product_id) $(busno) $(devno)";
        removal"/scripts/service-cellular.sh stop qwf-crm-telit";
    };
    -- Don't try matching any rules for the other interfaces
    interface(1,127) {
        Ignore;
    };
};

-- Cinterion/Gemalto AHS3 HSPA Modem modem module (Cinterion AHS3)
device(0x1e2d, 0x0055) {
    interface(0) {
--      driver"/scripts/service-cellular.sh start qwf-crm-gemalto ahs3 ppp0 $(vendor_id) $(product_id) $(busno) $(devno)";
        start"/scripts/service-cellular.sh start qwf-crm-gemalto ahs3 ppp0 $(vendor_id) $(product_id) $(busno) $(devno)";
        removal"/scripts/service-cellular.sh stop";
    };
    -- Don't try matching any rules for the other interfaces
    interface(1,127) {
        Ignore;
    };
};

-- Cinterion/Gemalto ALS3 LTE Modem modem module (Cinterion ALS3)
device(0x1e2d, 0x0061) {
    interface(0) {
--      driver"/scripts/service-cellular.sh start qwf-crm-gemalto als3 ppp0 $(vendor_id) $(product_id) $(busno) $(devno)";
        start"/scripts/service-cellular.sh start qwf-crm-gemalto als3 ppp0 $(vendor_id) $(product_id) $(busno) $(devno)";
        removal"/scripts/service-cellular.sh stop";
    };
    -- Don't try matching any rules for the other interfaces
    interface(1,127) {
        Ignore;
    };
};

-- ******************* THIS IS RELEVANT ONLY FOR START-OF-WORLD ********************
-- Factory default settings for the TOBY-L20x has it enumerate as product ID 0x1146
-- after briefly enumerating as 0x1140. QWF reconfigures resulting in the module then
-- enumerating as product ID 0x1143.
-- *********************************************************************************
---- u-blox TOBY-L20x modem module (RNDIS (only) + 1 CDC-ACM) (Marvell)
--device(0x1546, 0x1146) {
----  driver"/scripts/service-cellular.sh start qwf-crm-ublox l200 ecm0 $(vendor_id) $(product_id) $(busno) $(devno)";
--    start"/scripts/service-cellular.sh start qwf-crm-ublox l200 ecm0 $(vendor_id) $(product_id) $(busno) $(devno)";
--    removal"/scripts/service-cellular.sh stop";
--};

-- u-blox TOBY L20x modem module (1 CDC-ECM only + 3 CDC-ACM) (Marvell)
-- NOTE - This rule handles both the TOBY L200 and TOBY L201
device(0x1546, 0x1143) {
    interface(0) {
--      driver"/scripts/service-cellular.sh start qwf-crm-ublox l200 ecm0 $(vendor_id) $(product_id) $(busno) $(devno)";
        start"/scripts/service-cellular.sh start qwf-crm-ublox l200 ecm0 $(vendor_id) $(product_id) $(busno) $(devno)";
        removal"/scripts/service-cellular.sh stop";
    };
    -- Don't try matching any rules for the other interfaces
    interface(1,127) {
        Ignore;
    };
};

-- u-blox TOBY L4x06 modem modules (1 CDC-NCM + 4 CDC-ACM) (Intel)
device(0x1546, 0x1010) {
    interface(0) {
--      driver"/scripts/service-cellular.sh start qwf-crm-ublox l4006 ncm0 $(vendor_id) $(product_id) $(busno) $(devno)";
        start"/scripts/service-cellular.sh start qwf-crm-ublox l4006 ncm0 $(vendor_id) $(product_id) $(busno) $(devno)";
        removal"/scripts/service-cellular.sh stop";
    };
    -- Don't try matching any rules for the other interfaces
    interface(1,127) {
        Ignore;
    };
};

-- u-blox TOBY R20x modem module (6 CDC-ACM) (? chipset)
device(0x1546, 0x1107) {
    interface(0) {
--      driver"/scripts/service-cellular.sh start qwf-crm-ublox r200 ppp0 $(vendor_id) $(product_id) $(busno) $(devno)";
        start"/scripts/service-cellular.sh start qwf-crm-ublox r200 ppp0 $(vendor_id) $(product_id) $(busno) $(devno)";
        removal"/scripts/service-cellular.sh stop";
    };
    -- Don't try matching any rules for the other interfaces
    interface(1,127) {
        Ignore;
    };
};

-- ******************* THIS IS RELEVANT ONLY FOR START-OF-WORLD ********************
-- Factory default settings for the LISA-U20x has it enumerate as product ID 0x1102.
-- QWF reconfigures resulting in the module then enumerating as product ID 0x1104.
-- *********************************************************************************
---- u-blox LISA-U20x modem module (7 CDC-ACM) (Intel)
--device(0x1546, 0x1102) {
----  driver"/scripts/service-cellular.sh start qwf-crm-ublox u200 ecm0 $(vendor_id) $(product_id) $(busno) $(devno)";
--    start"/scripts/service-cellular.sh start qwf-crm-ublox u200 ecm0 $(vendor_id) $(product_id) $(busno) $(devno)";
--    removal"/scripts/service-cellular.sh stop";
--};

-- u-blox LISA-U20x modem module (1 CDC-ECM only + 4 CDC-ACM) (Intel)
-- NOTE - This rule handles both the LISA U200 and LISA U201
-- NOTE - the LISA-U20x requires a DHCP client to be run, which handles all net_pps control messaging.  therefore,
--        the 'auto' argument is provided to service-cellular.sh to have QWF cellular control started in auto (DHCP client) mode.
device(0x1546, 0x1104) {
    interface(0) {
--      driver"/scripts/service-cellular.sh start qwf-crm-ublox u200 ecm0 $(vendor_id) $(product_id) $(busno) $(devno) auto";
        start"/scripts/service-cellular.sh start qwf-crm-ublox u200 ecm0 $(vendor_id) $(product_id) $(busno) $(devno) auto";
        removal"/scripts/service-cellular.sh stop";
    };
    -- Don't try matching any rules for the other interfaces
    interface(1,127) {
        Ignore;
    };
};

--
-- QWF supported GPS modules/dongles
--

-- Ublox EVA-M8M GNSS receiver
device(0x1546, 0x01a8) {
    interface(0) {
        start"/scripts/service-gps.sh start ublox $(vendor_id) $(product_id) $(busno) $(devno)";
        removal"/scripts/service-gps.sh stop";
    };
    -- Don't try matching any rules for the other interfaces
    interface(1,127) {
        Ignore;
    };
};

if verbose >= 3 then
    table.dump(new_conf)
    table.dump(new_conf.flags)
    show_config()
end

