{
    "patcher": {
        "fileversion": 1,
        "appversion": {
            "major": 9,
            "minor": 2,
            "revision": 0,
            "architecture": "x64",
            "modernui": 1
        },
        "classnamespace": "box",
        "rect": [ 1658.0, -371.0, 1237.0, 1234.0 ],
        "openinpresentation": 1,
        "boxes": [
            {
                "box": {
                    "activebgcolor": [ 0.101960784313725, 0.43921568627451, 0.537254901960784, 0.44 ],
                    "id": "obj-3",
                    "maxclass": "live.button",
                    "numinlets": 1,
                    "numoutlets": 1,
                    "outlettype": [ "" ],
                    "parameter_enable": 1,
                    "patching_rect": [ 196.62921348314606, 316.85393258426967, 37.414965986394556, 36.734693877551024 ],
                    "presentation": 1,
                    "presentation_rect": [ 196.62921348314606, 316.85393258426967, 37.414965986394556, 36.734693877551024 ],
                    "saved_attribute_attributes": {
                        "activebgcolor": {
                            "expression": ""
                        },
                        "valueof": {
                            "parameter_enum": [ "off", "on" ],
                            "parameter_longname": "live.button[8]",
                            "parameter_mmax": 1,
                            "parameter_modmode": 0,
                            "parameter_shortname": "live.button",
                            "parameter_type": 2
                        }
                    },
                    "varname": "live.button[3]"
                }
            },
            {
                "box": {
                    "fontface": 1,
                    "fontsize": 16.0,
                    "id": "obj-1",
                    "maxclass": "comment",
                    "numinlets": 1,
                    "numoutlets": 0,
                    "patching_rect": [ 87.64044943820224, 350.56179775280896, 99.99999999999987, 24.0 ],
                    "presentation": 1,
                    "presentation_rect": [ 85.0, 336.0, 99.99999999999987, 24.0 ],
                    "text": "Configs"
                }
            },
            {
                "box": {
                    "id": "obj-78",
                    "maxclass": "newobj",
                    "numinlets": 1,
                    "numoutlets": 1,
                    "outlettype": [ "" ],
                    "patching_rect": [ 1154.0, 925.5605381165919, 56.0, 22.0 ],
                    "text": "pcontrol"
                }
            },
            {
                "box": {
                    "id": "obj-77",
                    "maxclass": "newobj",
                    "numinlets": 2,
                    "numoutlets": 2,
                    "outlettype": [ "bang", "" ],
                    "patching_rect": [ 1749.3273542600898, 695.9641255605382, 34.0, 22.0 ],
                    "text": "sel 4"
                }
            },
            {
                "box": {
                    "id": "obj-75",
                    "maxclass": "newobj",
                    "numinlets": 2,
                    "numoutlets": 2,
                    "outlettype": [ "bang", "" ],
                    "patching_rect": [ 1569.506726457399, 662.3318385650224, 34.0, 22.0 ],
                    "text": "sel 3"
                }
            },
            {
                "box": {
                    "id": "obj-76",
                    "maxclass": "newobj",
                    "numinlets": 2,
                    "numoutlets": 2,
                    "outlettype": [ "bang", "" ],
                    "patching_rect": [ 1425.560538116592, 626.0089686098655, 34.0, 22.0 ],
                    "text": "sel 2"
                }
            },
            {
                "box": {
                    "id": "obj-74",
                    "maxclass": "newobj",
                    "numinlets": 2,
                    "numoutlets": 2,
                    "outlettype": [ "bang", "" ],
                    "patching_rect": [ 1288.340807174888, 589.6860986547085, 34.0, 22.0 ],
                    "text": "sel 1"
                }
            },
            {
                "box": {
                    "id": "obj-73",
                    "maxclass": "message",
                    "numinlets": 2,
                    "numoutlets": 1,
                    "outlettype": [ "" ],
                    "patching_rect": [ 1749.3273542600898, 730.9417040358744, 187.0, 22.0 ],
                    "text": "spectral-centroid-panning.maxpat"
                }
            },
            {
                "box": {
                    "id": "obj-72",
                    "maxclass": "message",
                    "numinlets": 2,
                    "numoutlets": 1,
                    "outlettype": [ "" ],
                    "patching_rect": [ 1569.506726457399, 696.8609865470852, 177.0, 22.0 ],
                    "text": "spectral-flatness-breath.maxpat"
                }
            },
            {
                "box": {
                    "id": "obj-71",
                    "maxclass": "message",
                    "numinlets": 2,
                    "numoutlets": 1,
                    "outlettype": [ "" ],
                    "patching_rect": [ 1425.560538116592, 660.9865470852018, 135.0, 22.0 ],
                    "text": "zerr-cross-jump.maxpat"
                }
            },
            {
                "box": {
                    "id": "obj-70",
                    "maxclass": "message",
                    "numinlets": 2,
                    "numoutlets": 1,
                    "outlettype": [ "" ],
                    "patching_rect": [ 1288.340807174888, 624.6636771300449, 130.0, 22.0 ],
                    "text": "full-chain-quad.maxpat"
                }
            },
            {
                "box": {
                    "id": "obj-69",
                    "linecount": 3,
                    "maxclass": "message",
                    "numinlets": 2,
                    "numoutlets": 1,
                    "outlettype": [ "" ],
                    "patching_rect": [ 1154.0, 866.8161434977578, 170.35175879396985, 49.0 ],
                    "text": "load spectral-flatness-breath.maxpat"
                }
            },
            {
                "box": {
                    "id": "obj-67",
                    "maxclass": "message",
                    "numinlets": 2,
                    "numoutlets": 1,
                    "outlettype": [ "" ],
                    "patching_rect": [ 1153.0, 589.0, 126.0, 22.0 ],
                    "text": "custom-layout.maxpat"
                }
            },
            {
                "box": {
                    "id": "obj-65",
                    "maxclass": "newobj",
                    "numinlets": 1,
                    "numoutlets": 2,
                    "outlettype": [ "bang", "int" ],
                    "patching_rect": [ 1153.0, 476.0, 29.5, 22.0 ],
                    "text": "t b i"
                }
            },
            {
                "box": {
                    "id": "obj-62",
                    "maxclass": "newobj",
                    "numinlets": 2,
                    "numoutlets": 2,
                    "outlettype": [ "bang", "" ],
                    "patching_rect": [ 1153.0, 554.0, 34.0, 22.0 ],
                    "text": "sel 0"
                }
            },
            {
                "box": {
                    "id": "obj-61",
                    "maxclass": "message",
                    "numinlets": 2,
                    "numoutlets": 1,
                    "outlettype": [ "" ],
                    "patching_rect": [ 1169.9551569506727, 804.0358744394618, 48.0, 22.0 ],
                    "text": "load $1"
                }
            },
            {
                "box": {
                    "id": "obj-59",
                    "maxclass": "message",
                    "numinlets": 2,
                    "numoutlets": 1,
                    "outlettype": [ "" ],
                    "patching_rect": [ 1153.0, 509.0, 50.0, 22.0 ],
                    "text": "3"
                }
            },
            {
                "box": {
                    "fontsize": 14.0,
                    "id": "obj-57",
                    "maxclass": "live.menu",
                    "numinlets": 1,
                    "numoutlets": 3,
                    "outlettype": [ "", "", "float" ],
                    "parameter_enable": 1,
                    "patching_rect": [ 866.9999999999999, 619.0, 138.0000000000001, 20.0 ],
                    "presentation": 1,
                    "presentation_rect": [ 866.9999999999999, 614.0, 138.0000000000001, 20.0 ],
                    "saved_attribute_attributes": {
                        "valueof": {
                            "parameter_enum": [ "Custom layout", "Full chain quad", "Zero cross jump", "Spectral flatness breath", "Spectral centroid panning" ],
                            "parameter_longname": "live.menu",
                            "parameter_mmax": 4,
                            "parameter_modmode": 0,
                            "parameter_shortname": "live.menu",
                            "parameter_type": 2
                        }
                    },
                    "varname": "live.menu"
                }
            },
            {
                "box": {
                    "activebgcolor": [ 0.101960784313725, 0.43921568627451, 0.537254901960784, 0.44 ],
                    "id": "obj-51",
                    "maxclass": "live.button",
                    "numinlets": 1,
                    "numoutlets": 1,
                    "outlettype": [ "" ],
                    "parameter_enable": 1,
                    "patching_rect": [ 879.0702947845806, 570.0, 37.414965986394556, 36.734693877551024 ],
                    "presentation": 1,
                    "presentation_rect": [ 879.0702947845806, 570.0, 37.414965986394556, 36.734693877551024 ],
                    "saved_attribute_attributes": {
                        "activebgcolor": {
                            "expression": ""
                        },
                        "valueof": {
                            "parameter_enum": [ "off", "on" ],
                            "parameter_longname": "live.button[1]",
                            "parameter_mmax": 1,
                            "parameter_modmode": 0,
                            "parameter_shortname": "live.button",
                            "parameter_type": 2
                        }
                    },
                    "varname": "live.button[2]"
                }
            },
            {
                "box": {
                    "fontsize": 11.0,
                    "id": "obj-50",
                    "maxclass": "comment",
                    "numinlets": 1,
                    "numoutlets": 0,
                    "patching_rect": [ 22.0, 1051.0, 158.0, 19.0 ],
                    "presentation": 1,
                    "presentation_rect": [ 22.0, 1051.0, 158.0, 19.0 ],
                    "text": "github.com/ringbuffer-org/Zerr",
                    "textcolor": [ 0.647058823529412, 0.647058823529412, 0.647058823529412, 1.0 ]
                }
            },
            {
                "box": {
                    "fontsize": 11.0,
                    "id": "obj-46",
                    "maxclass": "comment",
                    "numinlets": 1,
                    "numoutlets": 0,
                    "patching_rect": [ 22.0, 1072.0, 482.0, 19.0 ],
                    "presentation": 1,
                    "presentation_rect": [ 22.0, 1072.0, 482.0, 19.0 ],
                    "text": "Yang, Zeyu & von Coler, Henrik. (2025). Zerr~ Autogenous Spatialization in PD and Max. ",
                    "textcolor": [ 0.647058823529412, 0.647058823529412, 0.647058823529412, 1.0 ]
                }
            },
            {
                "box": {
                    "activebgcolor": [ 0.0, 0.8274509803921568, 0.5529411764705883, 0.52 ],
                    "id": "obj-42",
                    "maxclass": "live.button",
                    "numinlets": 1,
                    "numoutlets": 1,
                    "outlettype": [ "" ],
                    "parameter_enable": 1,
                    "patching_rect": [ 396.59863945578235, 797.7777777777777, 37.414965986394556, 36.734693877551024 ],
                    "presentation": 1,
                    "presentation_rect": [ 396.59863945578235, 797.7777777777777, 37.414965986394556, 36.734693877551024 ],
                    "saved_attribute_attributes": {
                        "activebgcolor": {
                            "expression": ""
                        },
                        "valueof": {
                            "parameter_enum": [ "off", "on" ],
                            "parameter_longname": "live.button[7]",
                            "parameter_mmax": 1,
                            "parameter_modmode": 0,
                            "parameter_shortname": "live.button",
                            "parameter_type": 2
                        }
                    },
                    "varname": "live.button[1]"
                }
            },
            {
                "box": {
                    "activebgcolor": [ 0.0, 0.8274509803921568, 0.5529411764705883, 0.52 ],
                    "id": "obj-41",
                    "maxclass": "live.button",
                    "numinlets": 1,
                    "numoutlets": 1,
                    "outlettype": [ "" ],
                    "parameter_enable": 1,
                    "patching_rect": [ 203.75, 665.0, 37.414965986394556, 36.734693877551024 ],
                    "presentation": 1,
                    "presentation_rect": [ 203.75, 665.0, 37.414965986394556, 36.734693877551024 ],
                    "saved_attribute_attributes": {
                        "activebgcolor": {
                            "expression": ""
                        },
                        "valueof": {
                            "parameter_enum": [ "off", "on" ],
                            "parameter_longname": "live.button[6]",
                            "parameter_mmax": 1,
                            "parameter_modmode": 0,
                            "parameter_shortname": "live.button",
                            "parameter_type": 2
                        }
                    },
                    "varname": "live.button[6]"
                }
            },
            {
                "box": {
                    "activebgcolor": [ 0.0, 0.8274509803921568, 0.5529411764705883, 0.52 ],
                    "id": "obj-40",
                    "maxclass": "live.button",
                    "numinlets": 1,
                    "numoutlets": 1,
                    "outlettype": [ "" ],
                    "parameter_enable": 1,
                    "patching_rect": [ 357.1836734693878, 226.0, 37.414965986394556, 36.734693877551024 ],
                    "presentation": 1,
                    "presentation_rect": [ 357.1836734693878, 226.0, 37.414965986394556, 36.734693877551024 ],
                    "saved_attribute_attributes": {
                        "activebgcolor": {
                            "expression": ""
                        },
                        "valueof": {
                            "parameter_enum": [ "off", "on" ],
                            "parameter_longname": "live.button[5]",
                            "parameter_mmax": 1,
                            "parameter_modmode": 0,
                            "parameter_shortname": "live.button",
                            "parameter_type": 2
                        }
                    },
                    "varname": "live.button[5]"
                }
            },
            {
                "box": {
                    "activebgcolor": [ 0.0, 0.8274509803921568, 0.5529411764705883, 0.52 ],
                    "id": "obj-39",
                    "maxclass": "live.button",
                    "numinlets": 1,
                    "numoutlets": 1,
                    "outlettype": [ "" ],
                    "parameter_enable": 1,
                    "patching_rect": [ 728.75, 245.0, 37.414965986394556, 36.734693877551024 ],
                    "presentation": 1,
                    "presentation_rect": [ 728.75, 245.0, 37.414965986394556, 36.734693877551024 ],
                    "saved_attribute_attributes": {
                        "activebgcolor": {
                            "expression": ""
                        },
                        "valueof": {
                            "parameter_enum": [ "off", "on" ],
                            "parameter_longname": "live.button[4]",
                            "parameter_mmax": 1,
                            "parameter_modmode": 0,
                            "parameter_shortname": "live.button",
                            "parameter_type": 2
                        }
                    },
                    "varname": "live.button[4]"
                }
            },
            {
                "box": {
                    "fontname": "Futura Condensed ExtraBold",
                    "fontsize": 48.0,
                    "id": "obj-23",
                    "maxclass": "comment",
                    "numinlets": 1,
                    "numoutlets": 0,
                    "patching_rect": [ 22.0, 29.0, 1013.0, 68.0 ],
                    "presentation": 1,
                    "presentation_rect": [ 22.0, 29.0, 1013.0, 68.0 ],
                    "text": "Zerr*: Autogenous Spatialization System"
                }
            },
            {
                "box": {
                    "fontface": 0,
                    "fontsize": 24.0,
                    "id": "obj-2",
                    "linecount": 3,
                    "maxclass": "comment",
                    "numinlets": 1,
                    "numoutlets": 0,
                    "patching_rect": [ 22.0, 99.0, 823.0, 87.0 ],
                    "presentation": 1,
                    "presentation_linecount": 3,
                    "presentation_rect": [ 22.0, 99.0, 884.0, 87.0 ],
                    "text": "Channel-based spatialization for any loudspeaker layout. Audio features extracted from the signal drive its distribution over the speakers: no manual positioning. Four objects form the chain; each has a help patch."
                }
            },
            {
                "box": {
                    "id": "obj-5",
                    "maxclass": "newobj",
                    "numinlets": 1,
                    "numoutlets": 1,
                    "outlettype": [ "" ],
                    "patching_rect": [ 203.75, 1252.2222222222222, 56.0, 22.0 ],
                    "text": "pcontrol"
                }
            },
            {
                "box": {
                    "fontface": 1,
                    "fontsize": 16.0,
                    "id": "obj-6",
                    "maxclass": "comment",
                    "numinlets": 1,
                    "numoutlets": 0,
                    "patching_rect": [ 400.0, 228.33333333333326, 180.0, 24.0 ],
                    "presentation": 1,
                    "presentation_rect": [ 400.0, 228.33333333333326, 180.0, 24.0 ],
                    "text": "zerr.features~"
                }
            },
            {
                "box": {
                    "fontsize": 12.0,
                    "id": "obj-7",
                    "linecount": 5,
                    "maxclass": "comment",
                    "numinlets": 1,
                    "numoutlets": 0,
                    "patching_rect": [ 400.0, 253.8888888888888, 292.0, 74.0 ],
                    "presentation": 1,
                    "presentation_linecount": 5,
                    "presentation_rect": [ 400.0, 253.8888888888888, 292.0, 74.0 ],
                    "text": "Extracts audio features from a mono signal, at audio rate. Arguments pick the features (rms zcr flx ctd rlf cf flt zc); one signal outlet per argument, in argument order. These are the control signals that drive the spatialization."
                }
            },
            {
                "box": {
                    "id": "obj-8",
                    "maxclass": "message",
                    "numinlets": 2,
                    "numoutlets": 1,
                    "outlettype": [ "" ],
                    "patching_rect": [ 357.1836734693878, 1183.3333333333333, 140.0, 22.0 ],
                    "text": "help zerr.features~"
                }
            },
            {
                "box": {
                    "fontface": 1,
                    "fontsize": 16.0,
                    "id": "obj-9",
                    "maxclass": "comment",
                    "numinlets": 1,
                    "numoutlets": 0,
                    "patching_rect": [ 760.5555555555557, 278.88888888888886, 180.0, 24.0 ],
                    "presentation": 1,
                    "presentation_rect": [ 760.5555555555557, 278.88888888888886, 180.0, 24.0 ],
                    "text": "zerr.envelopes~"
                }
            },
            {
                "box": {
                    "fontsize": 12.0,
                    "id": "obj-10",
                    "linecount": 7,
                    "maxclass": "comment",
                    "numinlets": 1,
                    "numoutlets": 0,
                    "patching_rect": [ 760.5555555555557, 305.55555555555554, 278.0, 100.0 ],
                    "presentation": 1,
                    "presentation_linecount": 7,
                    "presentation_rect": [ 760.5555555555557, 305.55555555555554, 278.0, 100.0 ],
                    "text": "Turns a control signal plus a speaker-layout YAML into one envelope per loudspeaker. 'trigger' mode jumps to another speaker on each pulse; 'trajectory' mode pans along a chain of speakers. Spread and gain are audio-rate inlets; the active, traj, topo, curr and interval messages reshape the behaviour while it runs."
                }
            },
            {
                "box": {
                    "id": "obj-11",
                    "maxclass": "message",
                    "numinlets": 2,
                    "numoutlets": 1,
                    "outlettype": [ "" ],
                    "patching_rect": [ 728.75, 1144.4444444444443, 140.0, 22.0 ],
                    "text": "help zerr.envelopes~"
                }
            },
            {
                "box": {
                    "fontface": 1,
                    "fontsize": 16.0,
                    "id": "obj-12",
                    "maxclass": "comment",
                    "numinlets": 1,
                    "numoutlets": 0,
                    "patching_rect": [ 48.888888888888886, 671.3673469387757, 180.0, 24.0 ],
                    "presentation": 1,
                    "presentation_rect": [ 48.888888888888886, 671.3673469387757, 180.0, 24.0 ],
                    "text": "zerr.combinator~"
                }
            },
            {
                "box": {
                    "fontsize": 12.0,
                    "id": "obj-13",
                    "linecount": 6,
                    "maxclass": "comment",
                    "numinlets": 1,
                    "numoutlets": 0,
                    "patching_rect": [ 48.888888888888886, 703.7346938775511, 252.5, 87.0 ],
                    "presentation": 1,
                    "presentation_linecount": 6,
                    "presentation_rect": [ 48.888888888888886, 703.7346938775511, 252.5, 87.0 ],
                    "text": "Merges several envelope sets (one multichannel inlet per set, equal channel counts) into one set by add, root (geometric mean) or max. The mode is an argument, an attribute, and a message, so it can change while audio runs."
                }
            },
            {
                "box": {
                    "id": "obj-14",
                    "maxclass": "message",
                    "numinlets": 2,
                    "numoutlets": 1,
                    "outlettype": [ "" ],
                    "patching_rect": [ 203.75, 1155.5555555555554, 140.0, 22.0 ],
                    "text": "help zerr.combinator~"
                }
            },
            {
                "box": {
                    "fontface": 1,
                    "fontsize": 16.0,
                    "id": "obj-15",
                    "maxclass": "comment",
                    "numinlets": 1,
                    "numoutlets": 0,
                    "patching_rect": [ 292.22222222222223, 842.2222222222222, 180.0, 24.0 ],
                    "presentation": 1,
                    "presentation_rect": [ 292.22222222222223, 842.2222222222222, 180.0, 24.0 ],
                    "text": "zerr.disperser~"
                }
            },
            {
                "box": {
                    "fontsize": 12.0,
                    "id": "obj-16",
                    "linecount": 3,
                    "maxclass": "comment",
                    "numinlets": 1,
                    "numoutlets": 0,
                    "patching_rect": [ 292.22222222222223, 870.0, 275.0, 47.0 ],
                    "presentation": 1,
                    "presentation_linecount": 3,
                    "presentation_rect": [ 292.22222222222223, 870.0, 275.0, 47.0 ],
                    "text": "Multiplies a mono source by a multichannel envelope set: one output channel per loudspeaker, ready for mc.dac~ or further routing."
                }
            },
            {
                "box": {
                    "id": "obj-17",
                    "maxclass": "message",
                    "numinlets": 2,
                    "numoutlets": 1,
                    "outlettype": [ "" ],
                    "patching_rect": [ 396.59863945578235, 1138.888888888889, 140.0, 22.0 ],
                    "text": "help zerr.disperser~"
                }
            },
            {
                "box": {
                    "fontsize": 12.0,
                    "id": "obj-19",
                    "linecount": 4,
                    "maxclass": "comment",
                    "numinlets": 1,
                    "numoutlets": 0,
                    "patching_rect": [ 87.64044943820224, 376.40449438202245, 316.0, 60.0 ],
                    "presentation": 1,
                    "presentation_linecount": 4,
                    "presentation_rect": [ 85.0, 361.0, 305.3333333333333, 60.0 ],
                    "text": "Speaker layouts ship in the package's misc/ folder. name one as the second argument of zerr.envelopes~. You can also write your own YAML with a cartesian or spherical position per speaker."
                }
            },
            {
                "box": {
                    "fontsize": 11.0,
                    "id": "obj-20",
                    "maxclass": "comment",
                    "numinlets": 1,
                    "numoutlets": 0,
                    "patching_rect": [ 22.0, 1093.0, 740.0, 19.0 ],
                    "presentation": 1,
                    "presentation_rect": [ 22.0, 1093.0, 740.0, 19.0 ],
                    "text": "Z. Yang, H. von Coler. Autogenous Spatialization for Arbitrary Loudspeaker Setups (I3DA 2023).  ",
                    "textcolor": [ 0.647058823529412, 0.647058823529412, 0.647058823529412, 1.0 ]
                }
            },
            {
                "box": {
                    "fontface": 1,
                    "fontsize": 16.0,
                    "id": "obj-22",
                    "maxclass": "comment",
                    "numinlets": 1,
                    "numoutlets": 0,
                    "patching_rect": [ 765.0, 610.0, 99.99999999999987, 24.0 ],
                    "presentation": 1,
                    "presentation_rect": [ 765.0, 610.0, 99.99999999999987, 24.0 ],
                    "text": "Examples"
                }
            },
            {
                "box": {
                    "fontsize": 12.0,
                    "id": "obj-30",
                    "linecount": 3,
                    "maxclass": "comment",
                    "numinlets": 1,
                    "numoutlets": 0,
                    "patching_rect": [ 765.0, 636.0, 240.0, 47.0 ],
                    "presentation": 1,
                    "presentation_linecount": 3,
                    "presentation_rect": [ 765.0, 636.0, 240.0, 47.0 ],
                    "text": "Complete patches, one scenario each, in the package's examples/ folder (also listed under Examples in the Package Manager). "
                }
            },
            {
                "box": {
                    "autofit": 1,
                    "background": 1,
                    "forceaspect": 1,
                    "id": "obj-53",
                    "maxclass": "fpic",
                    "numinlets": 1,
                    "numoutlets": 1,
                    "outlettype": [ "jit_matrix" ],
                    "patching_rect": [ 22.0, 99.0, 1013.0000000000001, 1013.0000000000001 ],
                    "pic": "zerr-logo-grey-transparent.png",
                    "presentation": 1,
                    "presentation_rect": [ 22.0, 99.0, 1013.0000000000001, 1013.0000000000001 ]
                }
            }
        ],
        "lines": [
            {
                "patchline": {
                    "destination": [ "obj-5", 0 ],
                    "midpoints": [ 738.25, 1233.6128472222222, 213.25, 1233.6128472222222 ],
                    "source": [ "obj-11", 0 ]
                }
            },
            {
                "patchline": {
                    "destination": [ "obj-5", 0 ],
                    "source": [ "obj-14", 0 ]
                }
            },
            {
                "patchline": {
                    "destination": [ "obj-5", 0 ],
                    "midpoints": [ 406.09863945578235, 1206.5555555555557, 213.25, 1206.5555555555557 ],
                    "source": [ "obj-17", 0 ]
                }
            },
            {
                "patchline": {
                    "destination": [ "obj-11", 0 ],
                    "source": [ "obj-39", 0 ]
                }
            },
            {
                "patchline": {
                    "destination": [ "obj-8", 0 ],
                    "source": [ "obj-40", 0 ]
                }
            },
            {
                "patchline": {
                    "destination": [ "obj-14", 0 ],
                    "source": [ "obj-41", 0 ]
                }
            },
            {
                "patchline": {
                    "destination": [ "obj-17", 0 ],
                    "source": [ "obj-42", 0 ]
                }
            },
            {
                "patchline": {
                    "destination": [ "obj-69", 0 ],
                    "midpoints": [ 888.5702947845806, 614.772870651087, 1163.5, 614.772870651087 ],
                    "source": [ "obj-51", 0 ]
                }
            },
            {
                "patchline": {
                    "destination": [ "obj-65", 0 ],
                    "midpoints": [ 876.4999999999999, 635.45703125, 994.7587939698492, 635.45703125, 994.7587939698492, 461.85929648241205, 1162.5, 461.85929648241205 ],
                    "source": [ "obj-57", 0 ]
                }
            },
            {
                "patchline": {
                    "destination": [ "obj-62", 0 ],
                    "order": 4,
                    "source": [ "obj-59", 0 ]
                }
            },
            {
                "patchline": {
                    "destination": [ "obj-74", 0 ],
                    "midpoints": [ 1162.5, 543.8316984304932, 1297.840807174888, 543.8316984304932 ],
                    "order": 3,
                    "source": [ "obj-59", 0 ]
                }
            },
            {
                "patchline": {
                    "destination": [ "obj-75", 0 ],
                    "midpoints": [ 1162.5, 543.9002242152467, 1579.006726457399, 543.9002242152467 ],
                    "order": 1,
                    "source": [ "obj-59", 0 ]
                }
            },
            {
                "patchline": {
                    "destination": [ "obj-76", 0 ],
                    "midpoints": [ 1162.5, 543.7160874439461, 1435.060538116592, 543.7160874439461 ],
                    "order": 2,
                    "source": [ "obj-59", 0 ]
                }
            },
            {
                "patchline": {
                    "destination": [ "obj-77", 0 ],
                    "midpoints": [ 1162.5, 543.8487948430494, 1758.8273542600898, 543.8487948430494 ],
                    "order": 0,
                    "source": [ "obj-59", 0 ]
                }
            },
            {
                "patchline": {
                    "destination": [ "obj-69", 1 ],
                    "midpoints": [ 1179.4551569506727, 845.317982763453, 1314.8517587939698, 845.317982763453 ],
                    "source": [ "obj-61", 0 ]
                }
            },
            {
                "patchline": {
                    "destination": [ "obj-67", 0 ],
                    "source": [ "obj-62", 0 ]
                }
            },
            {
                "patchline": {
                    "destination": [ "obj-59", 1 ],
                    "midpoints": [ 1173.0, 503.5, 1193.5, 503.5 ],
                    "source": [ "obj-65", 1 ]
                }
            },
            {
                "patchline": {
                    "destination": [ "obj-59", 0 ],
                    "source": [ "obj-65", 0 ]
                }
            },
            {
                "patchline": {
                    "destination": [ "obj-61", 0 ],
                    "midpoints": [ 1162.5, 707.5179372197309, 1179.4551569506727, 707.5179372197309 ],
                    "source": [ "obj-67", 0 ]
                }
            },
            {
                "patchline": {
                    "destination": [ "obj-78", 0 ],
                    "source": [ "obj-69", 0 ]
                }
            },
            {
                "patchline": {
                    "destination": [ "obj-61", 0 ],
                    "midpoints": [ 1297.840807174888, 680.2369674887892, 1179.4551569506727, 680.2369674887892 ],
                    "source": [ "obj-70", 0 ]
                }
            },
            {
                "patchline": {
                    "destination": [ "obj-61", 0 ],
                    "midpoints": [ 1435.060538116592, 707.7926709641256, 1179.4551569506727, 707.7926709641256 ],
                    "source": [ "obj-71", 0 ]
                }
            },
            {
                "patchline": {
                    "destination": [ "obj-61", 0 ],
                    "midpoints": [ 1579.006726457399, 738.3577634529147, 1179.4551569506727, 738.3577634529147 ],
                    "source": [ "obj-72", 0 ]
                }
            },
            {
                "patchline": {
                    "destination": [ "obj-61", 0 ],
                    "midpoints": [ 1758.8273542600898, 764.2458660313902, 1179.4551569506727, 764.2458660313902 ],
                    "source": [ "obj-73", 0 ]
                }
            },
            {
                "patchline": {
                    "destination": [ "obj-70", 0 ],
                    "source": [ "obj-74", 0 ]
                }
            },
            {
                "patchline": {
                    "destination": [ "obj-72", 0 ],
                    "source": [ "obj-75", 0 ]
                }
            },
            {
                "patchline": {
                    "destination": [ "obj-71", 0 ],
                    "source": [ "obj-76", 0 ]
                }
            },
            {
                "patchline": {
                    "destination": [ "obj-73", 0 ],
                    "source": [ "obj-77", 0 ]
                }
            },
            {
                "patchline": {
                    "destination": [ "obj-5", 0 ],
                    "midpoints": [ 366.6836734693878, 1220.0668402777778, 213.25, 1220.0668402777778 ],
                    "source": [ "obj-8", 0 ]
                }
            }
        ],
        "parameters": {
            "obj-3": [ "live.button[8]", "live.button", 0 ],
            "obj-39": [ "live.button[4]", "live.button", 0 ],
            "obj-40": [ "live.button[5]", "live.button", 0 ],
            "obj-41": [ "live.button[6]", "live.button", 0 ],
            "obj-42": [ "live.button[7]", "live.button", 0 ],
            "obj-51": [ "live.button[1]", "live.button", 0 ],
            "obj-57": [ "live.menu", "live.menu", 0 ],
            "parameterbanks": {
                "0": {
                    "index": 0,
                    "name": "",
                    "parameters": [ "-", "-", "-", "-", "-", "-", "-", "-" ],
                    "buttons": [ "-", "-", "-", "-", "-", "-", "-", "-" ]
                }
            },
            "inherited_shortname": 1
        },
        "autosave": 0
    }
}