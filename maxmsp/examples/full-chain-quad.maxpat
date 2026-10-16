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
        "rect": [ 1863.0, -305.0, 1460.0, 1003.0 ],
        "boxes": [
            {
                "box": {
                    "id": "obj-48",
                    "lastchannelcount": 4,
                    "maxclass": "mc.live.gain~",
                    "numinlets": 1,
                    "numoutlets": 4,
                    "orientation": 1,
                    "outlettype": [ "multichannelsignal", "", "float", "list" ],
                    "parameter_enable": 1,
                    "patching_rect": [ 40.0, 829.3333333333334, 120.0, 59.0 ],
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
                    "id": "obj-46",
                    "maxclass": "newobj",
                    "numinlets": 1,
                    "numoutlets": 1,
                    "outlettype": [ "signal" ],
                    "patching_rect": [ 220.66666666666666, 634.1040462427744, 50.0, 22.0 ],
                    "text": "sig~ 1"
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
                    "outlettype": [ "jit_matrix" ],
                    "patching_rect": [ 828.0, 32.8, 103.20000153779984, 103.20000153779984 ],
                    "pic": "zerr-logo-white-transparent.png"
                }
            },
            {
                "box": {
                    "id": "obj-43",
                    "justification": 1,
                    "maxclass": "live.line",
                    "numinlets": 1,
                    "numoutlets": 0,
                    "patching_rect": [ 40.0, 182.8, 891.2000015377998, 13.432835820895521 ]
                }
            },
            {
                "box": {
                    "fontsize": 64.0,
                    "id": "obj-44",
                    "maxclass": "comment",
                    "numinlets": 1,
                    "numoutlets": 0,
                    "patching_rect": [ 40.0, 32.8, 862.0, 78.0 ],
                    "text": "Full chain on four speakers"
                }
            },
            {
                "box": {
                    "id": "obj-42",
                    "maxclass": "meter~",
                    "numinlets": 1,
                    "numleds": 20,
                    "numoutlets": 1,
                    "outlettype": [ "float" ],
                    "patching_rect": [ 534.5, 738.6666666666666, 146.0, 201.0 ]
                }
            },
            {
                "box": {
                    "fontsize": 12.0,
                    "id": "obj-2",
                    "linecount": 3,
                    "maxclass": "comment",
                    "numinlets": 1,
                    "numoutlets": 0,
                    "patching_rect": [ 40.0, 112.8, 808.5, 47.0 ],
                    "text": "Two features of one source shape its distribution over four loudspeakers. Loudness (rms) pans the sound along the speakers in one zerr.envelopes~, brightness (ctd) in a second one, and zerr.combinator~ in root mode keeps only the speakers both agree on. zerr.disperser~ applies the result to the source. You hear the sound wander between the four speakers, following the slow loudness and brightness wobble of the source."
                }
            },
            {
                "box": {
                    "fontsize": 11.0,
                    "id": "obj-3",
                    "maxclass": "comment",
                    "numinlets": 1,
                    "numoutlets": 0,
                    "patching_rect": [ 40.0, 161.8, 760.0, 19.0 ],
                    "text": "The chain of every Zerr* patch: zerr.features~ -> zerr.envelopes~ -> zerr.combinator~ -> zerr.disperser~ -> mc.dac~",
                    "textcolor": [ 0.647, 0.647, 0.647, 1.0 ]
                }
            },
            {
                "box": {
                    "fontsize": 11.0,
                    "id": "obj-4",
                    "maxclass": "comment",
                    "numinlets": 1,
                    "numoutlets": 0,
                    "patching_rect": [ 40.0, 198.8505747126437, 500.0, 19.0 ],
                    "text": "source: a saw through a low-pass, with slow wobbles on loudness and brightness"
                }
            },
            {
                "box": {
                    "id": "obj-5",
                    "maxclass": "newobj",
                    "numinlets": 2,
                    "numoutlets": 1,
                    "outlettype": [ "signal" ],
                    "patching_rect": [ 40.0, 224.13793103448276, 80.0, 22.0 ],
                    "text": "phasor~ 55"
                }
            },
            {
                "box": {
                    "id": "obj-6",
                    "maxclass": "newobj",
                    "numinlets": 2,
                    "numoutlets": 1,
                    "outlettype": [ "signal" ],
                    "patching_rect": [ 142.29885057471265, 251.14942528735634, 80.0, 22.0 ],
                    "text": "cycle~ 0.07"
                }
            },
            {
                "box": {
                    "id": "obj-7",
                    "maxclass": "newobj",
                    "numinlets": 2,
                    "numoutlets": 1,
                    "outlettype": [ "signal" ],
                    "patching_rect": [ 142.29885057471265, 281.0344827586207, 60.0, 22.0 ],
                    "text": "*~ 600."
                }
            },
            {
                "box": {
                    "id": "obj-8",
                    "maxclass": "newobj",
                    "numinlets": 2,
                    "numoutlets": 1,
                    "outlettype": [ "signal" ],
                    "patching_rect": [ 208.39080459770113, 281.0344827586207, 60.0, 22.0 ],
                    "text": "+~ 900."
                }
            },
            {
                "box": {
                    "fontsize": 11.0,
                    "id": "obj-9",
                    "maxclass": "comment",
                    "numinlets": 1,
                    "numoutlets": 0,
                    "patching_rect": [ 142.29885057471265, 225.28735632183907, 138.0, 19.0 ],
                    "text": "cutoff wobble (brightness)"
                }
            },
            {
                "box": {
                    "id": "obj-10",
                    "maxclass": "newobj",
                    "numinlets": 2,
                    "numoutlets": 1,
                    "outlettype": [ "signal" ],
                    "patching_rect": [ 302.64367816091954, 251.14942528735634, 80.0, 22.0 ],
                    "text": "cycle~ 0.2"
                }
            },
            {
                "box": {
                    "id": "obj-11",
                    "maxclass": "newobj",
                    "numinlets": 2,
                    "numoutlets": 1,
                    "outlettype": [ "signal" ],
                    "patching_rect": [ 302.64367816091954, 281.0344827586207, 60.0, 22.0 ],
                    "text": "*~ 0.4"
                }
            },
            {
                "box": {
                    "id": "obj-12",
                    "maxclass": "newobj",
                    "numinlets": 2,
                    "numoutlets": 1,
                    "outlettype": [ "signal" ],
                    "patching_rect": [ 367.0114942528736, 281.0344827586207, 60.0, 22.0 ],
                    "text": "+~ 0.6"
                }
            },
            {
                "box": {
                    "fontsize": 11.0,
                    "id": "obj-13",
                    "maxclass": "comment",
                    "numinlets": 1,
                    "numoutlets": 0,
                    "patching_rect": [ 302.64367816091954, 225.28735632183907, 160.0, 19.0 ],
                    "text": "level wobble (loudness)"
                }
            },
            {
                "box": {
                    "id": "obj-14",
                    "maxclass": "newobj",
                    "numinlets": 3,
                    "numoutlets": 1,
                    "outlettype": [ "signal" ],
                    "patching_rect": [ 40.0, 339.6551724137931, 100.0, 22.0 ],
                    "text": "lores~ 800 0.5"
                }
            },
            {
                "box": {
                    "id": "obj-15",
                    "maxclass": "newobj",
                    "numinlets": 2,
                    "numoutlets": 1,
                    "outlettype": [ "signal" ],
                    "patching_rect": [ 40.0, 389.6551724137931, 40.0, 22.0 ],
                    "text": "*~"
                }
            },
            {
                "box": {
                    "id": "obj-16",
                    "maxclass": "newobj",
                    "numinlets": 2,
                    "numoutlets": 1,
                    "outlettype": [ "signal" ],
                    "patching_rect": [ 40.0, 424.71264367816093, 50.0, 22.0 ],
                    "text": "*~ 0.3"
                }
            },
            {
                "box": {
                    "id": "obj-17",
                    "maxclass": "newobj",
                    "numinlets": 1,
                    "numoutlets": 2,
                    "outlettype": [ "signal", "signal" ],
                    "patching_rect": [ 67.24137931034483, 462.06896551724145, 150.0, 22.0 ],
                    "text": "zerr.features~ rms ctd"
                }
            },
            {
                "box": {
                    "fontsize": 11.0,
                    "id": "obj-18",
                    "maxclass": "comment",
                    "numinlets": 1,
                    "numoutlets": 0,
                    "patching_rect": [ 224.13793103448276, 463.56896551724145, 300.0, 19.0 ],
                    "text": "two features, two outlets, argument order"
                }
            },
            {
                "box": {
                    "id": "obj-19",
                    "maxclass": "newobj",
                    "numinlets": 2,
                    "numoutlets": 1,
                    "outlettype": [ "signal" ],
                    "patching_rect": [ 67.24137931034483, 511.4942528735632, 50.0, 22.0 ],
                    "text": "/~ 0.3"
                }
            },
            {
                "box": {
                    "id": "obj-20",
                    "maxclass": "newobj",
                    "numinlets": 3,
                    "numoutlets": 1,
                    "outlettype": [ "signal" ],
                    "patching_rect": [ 67.24137931034483, 562.6436781609195, 70.0, 22.0 ],
                    "text": "clip~ 0. 1."
                }
            },
            {
                "box": {
                    "fontsize": 11.0,
                    "id": "obj-21",
                    "maxclass": "comment",
                    "numinlets": 1,
                    "numoutlets": 0,
                    "patching_rect": [ 85.05747126436782, 537.9310344827586, 139.94252873563218, 19.0 ],
                    "text": "loudness 0 to 0.3 -> 0 to 1"
                }
            },
            {
                "box": {
                    "id": "obj-22",
                    "maxclass": "newobj",
                    "numinlets": 2,
                    "numoutlets": 1,
                    "outlettype": [ "signal" ],
                    "patching_rect": [ 237.35632183908046, 511.49425287356325, 60.0, 22.0 ],
                    "text": "/~ 1500."
                }
            },
            {
                "box": {
                    "id": "obj-23",
                    "maxclass": "newobj",
                    "numinlets": 3,
                    "numoutlets": 1,
                    "outlettype": [ "signal" ],
                    "patching_rect": [ 237.35632183908046, 562.6436781609195, 70.0, 22.0 ],
                    "text": "clip~ 0. 1."
                }
            },
            {
                "box": {
                    "fontsize": 11.0,
                    "id": "obj-24",
                    "maxclass": "comment",
                    "numinlets": 1,
                    "numoutlets": 0,
                    "patching_rect": [ 253.757225433526, 537.9310344827586, 200.0, 19.0 ],
                    "text": "brightness 0 to 1500 Hz -> 0 to 1"
                }
            },
            {
                "box": {
                    "id": "obj-25",
                    "maxclass": "newobj",
                    "numinlets": 1,
                    "numoutlets": 1,
                    "outlettype": [ "signal" ],
                    "patching_rect": [ 471.7514450867052, 634.1040462427745, 50.0, 22.0 ],
                    "text": "sig~ 1"
                }
            },
            {
                "box": {
                    "id": "obj-27",
                    "maxclass": "newobj",
                    "numinlets": 3,
                    "numoutlets": 1,
                    "outlettype": [ "multichannelsignal" ],
                    "patching_rect": [ 67.24137931034483, 661.849710982659, 200.0, 22.0 ],
                    "text": "zerr.envelopes~ trajectory quad_4"
                }
            },
            {
                "box": {
                    "id": "obj-28",
                    "maxclass": "newobj",
                    "numinlets": 3,
                    "numoutlets": 1,
                    "outlettype": [ "multichannelsignal" ],
                    "patching_rect": [ 290.7514450867052, 661.8497109826588, 200.0, 22.0 ],
                    "text": "zerr.envelopes~ trajectory quad_4"
                }
            },
            {
                "box": {
                    "fontsize": 11.0,
                    "id": "obj-29",
                    "linecount": 3,
                    "maxclass": "comment",
                    "numinlets": 1,
                    "numoutlets": 0,
                    "patching_rect": [ 307.54022988505744, 613.1040462427746, 157.33333333333334, 43.0 ],
                    "text": "quad_4 ships in the package's misc folder: four speakers, four envelopes"
                }
            },
            {
                "box": {
                    "id": "obj-30",
                    "maxclass": "newobj",
                    "numinlets": 2,
                    "numoutlets": 1,
                    "outlettype": [ "multichannelsignal" ],
                    "patching_rect": [ 67.24137931034483, 730.0, 150.0, 22.0 ],
                    "text": "zerr.combinator~ 2 root"
                }
            },
            {
                "box": {
                    "id": "obj-31",
                    "maxclass": "message",
                    "numinlets": 2,
                    "numoutlets": 1,
                    "outlettype": [ "" ],
                    "patching_rect": [ 534.5, 392.33870967741933, 70.0, 22.0 ],
                    "text": "mode root"
                }
            },
            {
                "box": {
                    "id": "obj-32",
                    "maxclass": "message",
                    "numinlets": 2,
                    "numoutlets": 1,
                    "outlettype": [ "" ],
                    "patching_rect": [ 618.5483870967741, 392.3387096774194, 70.0, 22.0 ],
                    "text": "mode max"
                }
            },
            {
                "box": {
                    "id": "obj-33",
                    "maxclass": "message",
                    "numinlets": 2,
                    "numoutlets": 1,
                    "outlettype": [ "" ],
                    "patching_rect": [ 702.8225806451613, 392.33870967741933, 70.0, 22.0 ],
                    "text": "mode add"
                }
            },
            {
                "box": {
                    "fontsize": 11.0,
                    "id": "obj-34",
                    "linecount": 2,
                    "maxclass": "comment",
                    "numinlets": 1,
                    "numoutlets": 0,
                    "patching_rect": [ 534.5, 352.01612903225805, 314.0, 31.0 ],
                    "text": "root: a speaker plays only where loudness and brightness both put it. max: either one is enough. add: the sum, louder"
                }
            },
            {
                "box": {
                    "id": "obj-35",
                    "maxclass": "newobj",
                    "numinlets": 2,
                    "numoutlets": 1,
                    "outlettype": [ "multichannelsignal" ],
                    "patching_rect": [ 40.0, 788.5057471264367, 110.0, 22.0 ],
                    "text": "zerr.disperser~"
                }
            },
            {
                "box": {
                    "fontsize": 11.0,
                    "id": "obj-36",
                    "linecount": 2,
                    "maxclass": "comment",
                    "numinlets": 1,
                    "numoutlets": 0,
                    "patching_rect": [ 152.66666666666666, 779.5057471264369, 186.0, 31.0 ],
                    "text": "left: the source. right: the combined envelopes. out: four channels"
                }
            },
            {
                "box": {
                    "id": "obj-40",
                    "maxclass": "newobj",
                    "numinlets": 1,
                    "numoutlets": 0,
                    "patching_rect": [ 40.0, 911.3333333333334, 80.0, 22.0 ],
                    "text": "mc.dac~"
                }
            },
            {
                "box": {
                    "fontsize": 11.0,
                    "id": "obj-41",
                    "maxclass": "comment",
                    "numinlets": 1,
                    "numoutlets": 0,
                    "patching_rect": [ 126.0, 914.0, 300.0, 19.0 ],
                    "text": "channels in the order of quad_4.yaml"
                }
            }
        ],
        "lines": [
            {
                "patchline": {
                    "destination": [ "obj-11", 0 ],
                    "source": [ "obj-10", 0 ]
                }
            },
            {
                "patchline": {
                    "destination": [ "obj-12", 0 ],
                    "source": [ "obj-11", 0 ]
                }
            },
            {
                "patchline": {
                    "destination": [ "obj-15", 1 ],
                    "midpoints": [ 376.5114942528736, 375.62010596264383, 70.5, 375.62010596264383 ],
                    "source": [ "obj-12", 0 ]
                }
            },
            {
                "patchline": {
                    "destination": [ "obj-15", 0 ],
                    "source": [ "obj-14", 0 ]
                }
            },
            {
                "patchline": {
                    "destination": [ "obj-16", 0 ],
                    "source": [ "obj-15", 0 ]
                }
            },
            {
                "patchline": {
                    "destination": [ "obj-17", 0 ],
                    "order": 0,
                    "source": [ "obj-16", 0 ]
                }
            },
            {
                "patchline": {
                    "destination": [ "obj-35", 0 ],
                    "order": 1,
                    "source": [ "obj-16", 0 ]
                }
            },
            {
                "patchline": {
                    "destination": [ "obj-19", 0 ],
                    "source": [ "obj-17", 0 ]
                }
            },
            {
                "patchline": {
                    "destination": [ "obj-22", 0 ],
                    "midpoints": [ 207.74137931034483, 503.1341594827586, 246.85632183908046, 503.1341594827586 ],
                    "source": [ "obj-17", 1 ]
                }
            },
            {
                "patchline": {
                    "destination": [ "obj-20", 0 ],
                    "source": [ "obj-19", 0 ]
                }
            },
            {
                "patchline": {
                    "destination": [ "obj-27", 0 ],
                    "source": [ "obj-20", 0 ]
                }
            },
            {
                "patchline": {
                    "destination": [ "obj-23", 0 ],
                    "source": [ "obj-22", 0 ]
                }
            },
            {
                "patchline": {
                    "destination": [ "obj-28", 0 ],
                    "midpoints": [ 246.85632183908046, 613.5510659590725, 300.2514450867052, 613.5510659590725 ],
                    "source": [ "obj-23", 0 ]
                }
            },
            {
                "patchline": {
                    "destination": [ "obj-28", 2 ],
                    "source": [ "obj-25", 0 ]
                }
            },
            {
                "patchline": {
                    "destination": [ "obj-30", 0 ],
                    "source": [ "obj-27", 0 ]
                }
            },
            {
                "patchline": {
                    "destination": [ "obj-30", 1 ],
                    "source": [ "obj-28", 0 ]
                }
            },
            {
                "patchline": {
                    "destination": [ "obj-35", 1 ],
                    "midpoints": [ 76.74137931034483, 767.0864096681497, 140.5, 767.0864096681497 ],
                    "source": [ "obj-30", 0 ]
                }
            },
            {
                "patchline": {
                    "destination": [ "obj-30", 0 ],
                    "midpoints": [ 544.0, 710.2240423387096, 76.74137931034483, 710.2240423387096 ],
                    "source": [ "obj-31", 0 ]
                }
            },
            {
                "patchline": {
                    "destination": [ "obj-30", 0 ],
                    "midpoints": [ 628.0483870967741, 710.1224798387098, 76.74137931034483, 710.1224798387098 ],
                    "source": [ "obj-32", 0 ]
                }
            },
            {
                "patchline": {
                    "destination": [ "obj-30", 0 ],
                    "midpoints": [ 712.3225806451613, 710.1459173387096, 76.74137931034483, 710.1459173387096 ],
                    "source": [ "obj-33", 0 ]
                }
            },
            {
                "patchline": {
                    "destination": [ "obj-42", 0 ],
                    "midpoints": [ 49.5, 820.5057471264367, 417.6458333333333, 820.5057471264367, 417.6458333333333, 727.3333333333334, 544.0, 727.3333333333334 ],
                    "order": 0,
                    "source": [ "obj-35", 0 ]
                }
            },
            {
                "patchline": {
                    "destination": [ "obj-48", 0 ],
                    "order": 1,
                    "source": [ "obj-35", 0 ]
                }
            },
            {
                "patchline": {
                    "destination": [ "obj-27", 2 ],
                    "source": [ "obj-46", 0 ]
                }
            },
            {
                "patchline": {
                    "destination": [ "obj-40", 0 ],
                    "source": [ "obj-48", 0 ]
                }
            },
            {
                "patchline": {
                    "destination": [ "obj-14", 0 ],
                    "source": [ "obj-5", 0 ]
                }
            },
            {
                "patchline": {
                    "destination": [ "obj-7", 0 ],
                    "source": [ "obj-6", 0 ]
                }
            },
            {
                "patchline": {
                    "destination": [ "obj-8", 0 ],
                    "source": [ "obj-7", 0 ]
                }
            },
            {
                "patchline": {
                    "destination": [ "obj-14", 1 ],
                    "midpoints": [ 217.89080459770113, 320.35643408764383, 90.0, 320.35643408764383 ],
                    "source": [ "obj-8", 0 ]
                }
            }
        ],
        "parameters": {
            "obj-48": [ "mc.live.gain~", "mc.live.gain~", 0 ],
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