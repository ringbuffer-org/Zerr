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
        "rect": [
            1569.0,
            -283.0,
            1912.0,
            1189.0
        ],
        "boxes": [
            {
                "box": {
                    "fontsize": 14.0,
                    "id": "obj-29",
                    "linecount": 37,
                    "maxclass": "comment",
                    "numinlets": 1,
                    "numoutlets": 0,
                    "patching_rect": [
                        25.0,
                        209.66666666666666,
                        117.7485380116959,
                        585.0
                    ],
                    "text": "standard:\n  1:\n    position:\n      cartesian:\n        x: -2.0\n        y:  3.0\n        z:  1.2\n  2:\n    position:\n      cartesian:\n        x:  2.0\n        y:  3.0\n        z:  1.2\n  3:\n    position:\n      cartesian:\n        x:  3.0\n        y:  0.0\n        z:  1.2\n  4:\n    position:\n      cartesian:\n        x:  3.0\n        y: -3.0\n        z:  1.2\n  5:\n    position:\n      cartesian:\n        x: -3.0\n        y: -3.0\n        z:  1.2\n  6:\n    position:\n      cartesian:\n        x: -3.0\n        y:  0.0\n        z:  1.2"
                }
            },
            {
                "box": {
                    "id": "obj-28",
                    "maxclass": "meter~",
                    "numinlets": 1,
                    "numleds": 20,
                    "numoutlets": 1,
                    "outlettype": [
                        "float"
                    ],
                    "patching_rect": [
                        191.2280701754386,
                        594.1520467836258,
                        146.0,
                        201.0
                    ]
                }
            },
            {
                "box": {
                    "id": "obj-27",
                    "maxclass": "live.line",
                    "numinlets": 1,
                    "numoutlets": 0,
                    "patching_rect": [
                        154.97076023391813,
                        185.66666666666666,
                        5.0,
                        609.0701754385965
                    ]
                }
            },
            {
                "box": {
                    "fontsize": 14.0,
                    "id": "obj-26",
                    "linecount": 4,
                    "maxclass": "comment",
                    "numinlets": 1,
                    "numoutlets": 0,
                    "patching_rect": [
                        25.0,
                        114.66666666666666,
                        723.0,
                        69.0
                    ],
                    "text": "zerr.envelopes~ loads any layout YAML on Max's search path by name. my-room-6.yaml sits next to this patch (the examples folder is on the search path): six speakers around a listening position. Copy the file next to your own patch, rename it, edit the positions and the count, and give zerr.envelopes~ its name. You hear noise circle the room, speaker 1 to 6 and back to 1."
                }
            },
            {
                "box": {
                    "autofit": 1,
                    "forceaspect": 1,
                    "id": "obj-53",
                    "maxclass": "fpic",
                    "numinlets": 1,
                    "numoutlets": 1,
                    "outlettype": [
                        "jit_matrix"
                    ],
                    "patching_rect": [
                        745.0,
                        22.06666589776674,
                        103.20000153779984,
                        103.20000153779984
                    ],
                    "pic": "zerr-logo-white-transparent.png"
                }
            },
            {
                "box": {
                    "fontsize": 64.0,
                    "id": "obj-25",
                    "maxclass": "comment",
                    "numinlets": 1,
                    "numoutlets": 0,
                    "patching_rect": [
                        25.0,
                        34.666666666666664,
                        846.0,
                        78.0
                    ],
                    "text": "Your own speaker layout"
                }
            },
            {
                "box": {
                    "fontface": 1,
                    "fontsize": 14.0,
                    "id": "obj-3",
                    "maxclass": "comment",
                    "numinlets": 1,
                    "numoutlets": 0,
                    "patching_rect": [
                        25.0,
                        185.66666666666666,
                        250.0,
                        22.0
                    ],
                    "text": "my-room-6.yaml:"
                }
            },
            {
                "box": {
                    "fontsize": 14.0,
                    "id": "obj-5",
                    "linecount": 4,
                    "maxclass": "comment",
                    "numinlets": 1,
                    "numoutlets": 0,
                    "patching_rect": [
                        160.8187134502924,
                        185.66666666666666,
                        595.2046783625732,
                        69.0
                    ],
                    "text": "Metres from the listening position: x to the right, y to the front, z up. The speaker numbers are the channel order of the outlet and of mc.dac~. Spherical works too, with azimuth and elevation in degrees and a distance; see ring_8.yaml in the package's misc folder. Every speaker needs a number and a position; the file may list any count."
                }
            },
            {
                "box": {
                    "id": "obj-6",
                    "maxclass": "newobj",
                    "numinlets": 2,
                    "numoutlets": 1,
                    "outlettype": [
                        "signal"
                    ],
                    "patching_rect": [
                        282.4561403508772,
                        369.5906432748538,
                        80.0,
                        22.0
                    ],
                    "text": "phasor~ 0.1"
                }
            },
            {
                "box": {
                    "fontsize": 11.0,
                    "id": "obj-7",
                    "linecount": 2,
                    "maxclass": "comment",
                    "numinlets": 1,
                    "numoutlets": 0,
                    "patching_rect": [
                        282.4561403508772,
                        334.5029239766082,
                        239.76608187134502,
                        31.0
                    ],
                    "text": "position 0 to 1 sweeps the trajectory: speakers 1 to 6 in order, ten seconds per round"
                }
            },
            {
                "box": {
                    "id": "obj-10",
                    "maxclass": "newobj",
                    "numinlets": 1,
                    "numoutlets": 1,
                    "outlettype": [
                        "signal"
                    ],
                    "patching_rect": [
                        191.2280701754386,
                        359.06432748538015,
                        50.0,
                        22.0
                    ],
                    "text": "noise~"
                }
            },
            {
                "box": {
                    "id": "obj-11",
                    "maxclass": "newobj",
                    "numinlets": 2,
                    "numoutlets": 1,
                    "outlettype": [
                        "signal"
                    ],
                    "patching_rect": [
                        191.2280701754386,
                        388.30409356725147,
                        50.0,
                        22.0
                    ],
                    "text": "*~ 0.2"
                }
            },
            {
                "box": {
                    "fontsize": 11.0,
                    "id": "obj-12",
                    "maxclass": "comment",
                    "numinlets": 1,
                    "numoutlets": 0,
                    "patching_rect": [
                        191.2280701754386,
                        334.5029239766082,
                        60.0,
                        19.0
                    ],
                    "text": "source"
                }
            },
            {
                "box": {
                    "id": "obj-13",
                    "maxclass": "newobj",
                    "numinlets": 3,
                    "numoutlets": 1,
                    "outlettype": [
                        "multichannelsignal"
                    ],
                    "patching_rect": [
                        282.4561403508772,
                        464.9122807017544,
                        208.0,
                        22.0
                    ],
                    "text": "zerr.envelopes~ trajectory my-room-6"
                }
            },
            {
                "box": {
                    "id": "obj-14",
                    "maxclass": "message",
                    "numinlets": 2,
                    "numoutlets": 1,
                    "outlettype": [
                        ""
                    ],
                    "patching_rect": [
                        387.13450292397664,
                        397.0760233918129,
                        45.0,
                        22.0
                    ],
                    "text": "print"
                }
            },
            {
                "box": {
                    "id": "obj-15",
                    "maxclass": "message",
                    "numinlets": 2,
                    "numoutlets": 1,
                    "outlettype": [
                        ""
                    ],
                    "patching_rect": [
                        434.5029239766082,
                        397.0760233918129,
                        45.0,
                        22.0
                    ],
                    "text": "bang"
                }
            },
            {
                "box": {
                    "id": "obj-18",
                    "maxclass": "newobj",
                    "numinlets": 2,
                    "numoutlets": 1,
                    "outlettype": [
                        "multichannelsignal"
                    ],
                    "patching_rect": [
                        191.2280701754386,
                        525.7309941520468,
                        110.0,
                        22.0
                    ],
                    "text": "zerr.disperser~"
                }
            },
            {
                "box": {
                    "fontsize": 11.0,
                    "id": "obj-23",
                    "maxclass": "comment",
                    "numinlets": 1,
                    "numoutlets": 0,
                    "patching_rect": [
                        209.35672514619884,
                        563.7426900584795,
                        190.05847953216374,
                        19.0
                    ],
                    "text": "six channels, numbered as in the file"
                }
            },
            {
                "box": {
                    "fontsize": 11.0,
                    "id": "obj-24",
                    "linecount": 3,
                    "maxclass": "comment",
                    "numinlets": 1,
                    "numoutlets": 0,
                    "patching_rect": [
                        369.0058479532164,
                        490.05847953216374,
                        380.70175438596493,
                        43.0
                    ],
                    "text": "If the layout is not found, the object fails to create and the console says: cannot find speaker layout. Check the name, the extension (.yaml or .yml) a\nnd that the file is on the search path"
                }
            }
        ],
        "lines": [
            {
                "patchline": {
                    "destination": [
                        "obj-11",
                        0
                    ],
                    "source": [
                        "obj-10",
                        0
                    ]
                }
            },
            {
                "patchline": {
                    "destination": [
                        "obj-18",
                        0
                    ],
                    "source": [
                        "obj-11",
                        0
                    ]
                }
            },
            {
                "patchline": {
                    "destination": [
                        "obj-18",
                        1
                    ],
                    "source": [
                        "obj-13",
                        0
                    ]
                }
            },
            {
                "patchline": {
                    "destination": [
                        "obj-13",
                        0
                    ],
                    "midpoints": [
                        396.63450292397664,
                        435.2027595029238,
                        291.9561403508772,
                        435.2027595029238
                    ],
                    "source": [
                        "obj-14",
                        0
                    ]
                }
            },
            {
                "patchline": {
                    "destination": [
                        "obj-13",
                        0
                    ],
                    "midpoints": [
                        444.0029239766082,
                        434.66593567251454,
                        291.9561403508772,
                        434.66593567251454
                    ],
                    "source": [
                        "obj-15",
                        0
                    ]
                }
            },
            {
                "patchline": {
                    "destination": [
                        "obj-28",
                        0
                    ],
                    "source": [
                        "obj-18",
                        0
                    ]
                }
            },
            {
                "patchline": {
                    "destination": [
                        "obj-13",
                        0
                    ],
                    "source": [
                        "obj-6",
                        0
                    ]
                }
            }
        ],
        "autosave": 0
    }
}
