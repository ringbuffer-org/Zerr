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
            2015.0,
            -371.0,
            896.0,
            756.0
        ],
        "boxes": [
            {
                "box": {
                    "id": "obj-48",
                    "lastchannelcount": 8,
                    "maxclass": "mc.live.gain~",
                    "numinlets": 1,
                    "numoutlets": 4,
                    "outlettype": [
                        "multichannelsignal",
                        "",
                        "float",
                        "list"
                    ],
                    "parameter_enable": 1,
                    "patching_rect": [
                        39.39999923110008,
                        571.9137931034483,
                        98.0,
                        101.0
                    ],
                    "saved_attribute_attributes": {
                        "valueof": {
                            "parameter_longname": "mc.live.gain~",
                            "parameter_mmax": 6.0,
                            "parameter_mmin": -70.0,
                            "parameter_modmode": 3,
                            "parameter_shortname": "mc.live.gain~",
                            "parameter_type": 0,
                            "parameter_unitstyle": 4
                        }
                    },
                    "varname": "mc.live.gain~"
                }
            },
            {
                "box": {
                    "id": "obj-42",
                    "maxclass": "meter~",
                    "numinlets": 1,
                    "numleds": 20,
                    "numoutlets": 1,
                    "outlettype": [
                        "float"
                    ],
                    "patching_rect": [
                        364.367816091954,
                        547.9367816091952,
                        129.51494175983578,
                        167.816091954023
                    ]
                }
            },
            {
                "box": {
                    "id": "obj-4",
                    "maxclass": "newobj",
                    "numinlets": 1,
                    "numoutlets": 0,
                    "patching_rect": [
                        39.39999923110008,
                        693.7528735632184,
                        80.0,
                        22.0
                    ],
                    "text": "mc.dac~"
                }
            },
            {
                "box": {
                    "id": "obj-43",
                    "justification": 1,
                    "maxclass": "live.line",
                    "numinlets": 1,
                    "numoutlets": 0,
                    "patching_rect": [
                        39.39999923110008,
                        168.8,
                        891.2000015377998,
                        13.432835820895521
                    ]
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
                        828.0,
                        32.8,
                        103.20000153779984,
                        103.20000153779984
                    ],
                    "pic": "zerr-logo-white-transparent.png"
                }
            },
            {
                "box": {
                    "fontsize": 64.0,
                    "id": "obj-44",
                    "maxclass": "comment",
                    "numinlets": 1,
                    "numoutlets": 0,
                    "patching_rect": [
                        40.0,
                        32.8,
                        862.0,
                        78.0
                    ],
                    "text": "Spectral centroid panning"
                }
            },
            {
                "box": {
                    "fontsize": 12.0,
                    "id": "obj-2",
                    "linecount": 2,
                    "maxclass": "comment",
                    "numinlets": 1,
                    "numoutlets": 0,
                    "patching_rect": [
                        40.0,
                        112.8,
                        599.0,
                        33.0
                    ],
                    "text": "The spectral centroid of the source pans the sound around a ring of eight loudspeakers (trajectory mode). The source is two-operator FM: brighter spectra move the sound further along the ring."
                }
            },
            {
                "box": {
                    "fontsize": 11.0,
                    "id": "obj-3",
                    "maxclass": "comment",
                    "numinlets": 1,
                    "numoutlets": 0,
                    "patching_rect": [
                        40.0,
                        147.8,
                        760.0,
                        19.0
                    ],
                    "text": "https://en.wikipedia.org/wiki/Spectral_centroid",
                    "textcolor": [
                        0.647,
                        0.647,
                        0.647,
                        1.0
                    ]
                }
            },
            {
                "box": {
                    "fontface": 1,
                    "id": "obj-5",
                    "maxclass": "comment",
                    "numinlets": 1,
                    "numoutlets": 0,
                    "patching_rect": [
                        39.39999923110008,
                        184.23283582089553,
                        200.0,
                        20.0
                    ],
                    "text": "source: simple FM"
                }
            },
            {
                "box": {
                    "format": 6,
                    "id": "obj-6",
                    "maxclass": "flonum",
                    "maximum": 2000.0,
                    "minimum": 20.0,
                    "numinlets": 1,
                    "numoutlets": 2,
                    "outlettype": [
                        "",
                        "bang"
                    ],
                    "parameter_enable": 0,
                    "patching_rect": [
                        155.1724137931034,
                        286.2068965517241,
                        50.0,
                        22.0
                    ]
                }
            },
            {
                "box": {
                    "id": "obj-7",
                    "maxclass": "comment",
                    "numinlets": 1,
                    "numoutlets": 0,
                    "patching_rect": [
                        209.7701149425287,
                        287.9310344827586,
                        120.0,
                        20.0
                    ],
                    "text": "carrier (Hz)"
                }
            },
            {
                "box": {
                    "format": 6,
                    "id": "obj-8",
                    "maxclass": "flonum",
                    "maximum": 1000.0,
                    "minimum": 0.0,
                    "numinlets": 1,
                    "numoutlets": 2,
                    "outlettype": [
                        "",
                        "bang"
                    ],
                    "parameter_enable": 0,
                    "patching_rect": [
                        155.17241379310346,
                        213.21839080459773,
                        50.0,
                        22.0
                    ]
                }
            },
            {
                "box": {
                    "id": "obj-9",
                    "maxclass": "comment",
                    "numinlets": 1,
                    "numoutlets": 0,
                    "patching_rect": [
                        207.68735555293927,
                        215.21839080459773,
                        120.0,
                        20.0
                    ],
                    "text": "modulator (Hz)"
                }
            },
            {
                "box": {
                    "format": 6,
                    "id": "obj-10",
                    "maxclass": "flonum",
                    "maximum": 4000.0,
                    "minimum": 0.0,
                    "numinlets": 1,
                    "numoutlets": 2,
                    "outlettype": [
                        "",
                        "bang"
                    ],
                    "parameter_enable": 0,
                    "patching_rect": [
                        155.17241379310343,
                        250.8018013381369,
                        50.0,
                        22.0
                    ]
                }
            },
            {
                "box": {
                    "id": "obj-11",
                    "maxclass": "comment",
                    "numinlets": 1,
                    "numoutlets": 0,
                    "patching_rect": [
                        207.68735555293927,
                        251.8018013381369,
                        120.0,
                        20.0
                    ],
                    "text": "mod depth (Hz)"
                }
            },
            {
                "box": {
                    "id": "obj-12",
                    "maxclass": "newobj",
                    "numinlets": 2,
                    "numoutlets": 1,
                    "outlettype": [
                        "signal"
                    ],
                    "patching_rect": [
                        39.39999923110008,
                        222.30180133813687,
                        80.0,
                        22.0
                    ],
                    "text": "cycle~ 100"
                }
            },
            {
                "box": {
                    "id": "obj-13",
                    "maxclass": "newobj",
                    "numinlets": 2,
                    "numoutlets": 1,
                    "outlettype": [
                        "signal"
                    ],
                    "patching_rect": [
                        39.39999923110008,
                        270.65217391304344,
                        60.0,
                        22.0
                    ],
                    "text": "*~ 400."
                }
            },
            {
                "box": {
                    "id": "obj-14",
                    "maxclass": "newobj",
                    "numinlets": 2,
                    "numoutlets": 1,
                    "outlettype": [
                        "signal"
                    ],
                    "patching_rect": [
                        39.39999923110008,
                        327.01149425287355,
                        60.0,
                        22.0
                    ],
                    "text": "+~ 200."
                }
            },
            {
                "box": {
                    "id": "obj-15",
                    "maxclass": "newobj",
                    "numinlets": 2,
                    "numoutlets": 1,
                    "outlettype": [
                        "signal"
                    ],
                    "patching_rect": [
                        39.39999923110008,
                        360.3448275862069,
                        60.0,
                        22.0
                    ],
                    "text": "cycle~"
                }
            },
            {
                "box": {
                    "id": "obj-16",
                    "maxclass": "newobj",
                    "numinlets": 2,
                    "numoutlets": 1,
                    "outlettype": [
                        "signal"
                    ],
                    "patching_rect": [
                        39.39999923110008,
                        394.2528735632184,
                        60.0,
                        22.0
                    ],
                    "text": "*~ 0.3"
                }
            },
            {
                "box": {
                    "fontface": 1,
                    "id": "obj-17",
                    "maxclass": "comment",
                    "numinlets": 1,
                    "numoutlets": 0,
                    "patching_rect": [
                        419.2850567023645,
                        184.23283582089553,
                        300.0,
                        20.0
                    ],
                    "text": "feature -> 0..1 -> trajectory position"
                }
            },
            {
                "box": {
                    "id": "obj-18",
                    "maxclass": "newobj",
                    "numinlets": 1,
                    "numoutlets": 1,
                    "outlettype": [
                        "signal"
                    ],
                    "patching_rect": [
                        419.99999999999994,
                        222.30180133813687,
                        130.0,
                        22.0
                    ],
                    "text": "zerr.features~ ctd"
                }
            },
            {
                "box": {
                    "id": "obj-19",
                    "maxclass": "newobj",
                    "numinlets": 2,
                    "numoutlets": 1,
                    "outlettype": [
                        "signal"
                    ],
                    "patching_rect": [
                        419.99999999999994,
                        257.3592726025047,
                        70.0,
                        22.0
                    ],
                    "text": "/~ 800."
                }
            },
            {
                "box": {
                    "id": "obj-20",
                    "maxclass": "newobj",
                    "numinlets": 3,
                    "numoutlets": 1,
                    "outlettype": [
                        "signal"
                    ],
                    "patching_rect": [
                        419.99999999999994,
                        292.4167438668725,
                        80.0,
                        22.0
                    ],
                    "text": "clip~ 0. 1."
                }
            },
            {
                "box": {
                    "fontsize": 11.0,
                    "id": "obj-21",
                    "linecount": 2,
                    "maxclass": "comment",
                    "numinlets": 1,
                    "numoutlets": 0,
                    "patching_rect": [
                        491.99999999999994,
                        252.85927260250463,
                        300.0,
                        31.0
                    ],
                    "text": "centroid in Hz divided by 800 and clipped: 0 Hz is speaker 1, 800 Hz and above is speaker 8"
                }
            },
            {
                "box": {
                    "id": "obj-22",
                    "maxclass": "newobj",
                    "numinlets": 3,
                    "numoutlets": 1,
                    "outlettype": [
                        "multichannelsignal"
                    ],
                    "patching_rect": [
                        419.2850567023645,
                        394.25287356321843,
                        200.0,
                        22.0
                    ],
                    "text": "zerr.envelopes~ trajectory ring_8"
                }
            },
            {
                "box": {
                    "id": "obj-23",
                    "maxclass": "newobj",
                    "numinlets": 2,
                    "numoutlets": 1,
                    "outlettype": [
                        "multichannelsignal"
                    ],
                    "patching_rect": [
                        39.39999923110008,
                        479.88505747126436,
                        110.0,
                        22.0
                    ],
                    "text": "zerr.disperser~"
                }
            }
        ],
        "lines": [
            {
                "patchline": {
                    "destination": [
                        "obj-13",
                        1
                    ],
                    "midpoints": [
                        164.67241379310343,
                        282.8018013381369,
                        127.28620651210176,
                        282.8018013381369,
                        127.28620651210176,
                        258.5138261850885,
                        89.89999923110008,
                        258.5138261850885
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
                        "obj-13",
                        0
                    ],
                    "source": [
                        "obj-12",
                        0
                    ]
                }
            },
            {
                "patchline": {
                    "destination": [
                        "obj-14",
                        0
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
                        "obj-15",
                        0
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
                        "obj-16",
                        0
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
                        "obj-18",
                        0
                    ],
                    "midpoints": [
                        48.89999923110008,
                        426.2528735632184,
                        355.0108110127093,
                        426.2528735632184,
                        355.0108110127093,
                        213.74208510250472,
                        429.49999999999994,
                        213.74208510250472
                    ],
                    "order": 0,
                    "source": [
                        "obj-16",
                        0
                    ]
                }
            },
            {
                "patchline": {
                    "destination": [
                        "obj-23",
                        0
                    ],
                    "order": 1,
                    "source": [
                        "obj-16",
                        0
                    ]
                }
            },
            {
                "patchline": {
                    "destination": [
                        "obj-19",
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
                        "obj-20",
                        0
                    ],
                    "source": [
                        "obj-19",
                        0
                    ]
                }
            },
            {
                "patchline": {
                    "destination": [
                        "obj-22",
                        0
                    ],
                    "source": [
                        "obj-20",
                        0
                    ]
                }
            },
            {
                "patchline": {
                    "destination": [
                        "obj-23",
                        1
                    ],
                    "midpoints": [
                        428.7850567023645,
                        449.1181240886087,
                        139.89999923110008,
                        449.1181240886087
                    ],
                    "source": [
                        "obj-22",
                        0
                    ]
                }
            },
            {
                "patchline": {
                    "destination": [
                        "obj-42",
                        0
                    ],
                    "midpoints": [
                        48.89999923110008,
                        525.7921156609195,
                        373.867816091954,
                        525.7921156609195
                    ],
                    "order": 0,
                    "source": [
                        "obj-23",
                        0
                    ]
                }
            },
            {
                "patchline": {
                    "destination": [
                        "obj-48",
                        0
                    ],
                    "order": 1,
                    "source": [
                        "obj-23",
                        0
                    ]
                }
            },
            {
                "patchline": {
                    "destination": [
                        "obj-4",
                        0
                    ],
                    "source": [
                        "obj-48",
                        0
                    ]
                }
            },
            {
                "patchline": {
                    "destination": [
                        "obj-14",
                        1
                    ],
                    "midpoints": [
                        164.6724137931034,
                        317.60919540229884,
                        89.89999923110008,
                        317.60919540229884
                    ],
                    "source": [
                        "obj-6",
                        0
                    ]
                }
            },
            {
                "patchline": {
                    "destination": [
                        "obj-12",
                        0
                    ],
                    "midpoints": [
                        164.67241379310346,
                        240.72620330459773,
                        139.5156425753202,
                        240.72620330459773,
                        139.5156425753202,
                        208.55019755747125,
                        48.89999923110008,
                        208.55019755747125
                    ],
                    "source": [
                        "obj-8",
                        0
                    ]
                }
            }
        ],
        "parameters": {
            "obj-48": [
                "mc.live.gain~",
                "mc.live.gain~",
                0
            ],
            "parameterbanks": {
                "0": {
                    "index": 0,
                    "name": "",
                    "parameters": [
                        "-",
                        "-",
                        "-",
                        "-",
                        "-",
                        "-",
                        "-",
                        "-"
                    ],
                    "buttons": [
                        "-",
                        "-",
                        "-",
                        "-",
                        "-",
                        "-",
                        "-",
                        "-"
                    ]
                }
            },
            "inherited_shortname": 1
        },
        "autosave": 0
    }
}
