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
        "rect": [ 1559.0, -370.0, 1428.0, 980.0 ],
        "boxes": [
            {
                "box": {
                    "fontsize": 14.0,
                    "id": "obj-66",
                    "linecount": 4,
                    "maxclass": "comment",
                    "numinlets": 1,
                    "numoutlets": 0,
                    "patching_rect": [ 34.63945578231293, 769.3877551020408, 705.0, 69.0 ],
                    "text": "Think of root as \"and\" and max as \"or\": root plays a speaker only where every feature wants it, which keeps the distribution focused, while max lets any feature claim a speaker, which keeps it wide. Add is the sum, so it can exceed 1, and [zerr.disperser~] does not limit; scale it down, for instance with the gain inlet of each [zerr.envelopes~] or an [mc.*~ 0.5] after the combinator."
                }
            },
            {
                "box": {
                    "fontsize": 14.0,
                    "id": "obj-65",
                    "linecount": 3,
                    "maxclass": "comment",
                    "numinlets": 1,
                    "numoutlets": 0,
                    "patching_rect": [ 36.0, 426.53061224489795, 421.7687074829932, 53.0 ],
                    "text": "Every inlet must carry the same channel count. Otherwise the object reports an error and outputs silence until the counts match. Feed it [zerr.envelopes~] objects loaded with the same layout."
                }
            },
            {
                "box": {
                    "fontsize": 14.0,
                    "id": "obj-58",
                    "linecount": 4,
                    "maxclass": "comment",
                    "numinlets": 1,
                    "numoutlets": 0,
                    "patching_rect": [ 608.7891156462584, 541.4965986394558, 313.0, 69.0 ],
                    "text": "Mode is live. mode add, mode root or mode max as a message, @mode as an attribute or the inspector all switch while audio runs, with no click."
                }
            },
            {
                "box": {
                    "fontsize": 14.0,
                    "id": "obj-57",
                    "linecount": 2,
                    "maxclass": "comment",
                    "numinlets": 1,
                    "numoutlets": 0,
                    "patching_rect": [ 617.6938775510203, 419.53061224489795, 237.0, 38.0 ],
                    "text": "bang posts the mode, the number of sets and the channels per set."
                }
            },
            {
                "box": {
                    "id": "obj-43",
                    "maxclass": "newobj",
                    "numinlets": 1,
                    "numoutlets": 1,
                    "outlettype": [ "multichannelsignal" ],
                    "patching_rect": [ 389.0612244897959, 481.6326530612245, 78.0, 22.0 ],
                    "text": "mc.r~ env2 3"
                }
            },
            {
                "box": {
                    "id": "obj-44",
                    "maxclass": "newobj",
                    "numinlets": 1,
                    "numoutlets": 1,
                    "outlettype": [ "multichannelsignal" ],
                    "patching_rect": [ 308.78911564625855, 481.6326530612245, 78.0, 22.0 ],
                    "text": "mc.r~ env1 3"
                }
            },
            {
                "box": {
                    "id": "obj-37",
                    "maxclass": "newobj",
                    "numinlets": 1,
                    "numoutlets": 1,
                    "outlettype": [ "multichannelsignal" ],
                    "patching_rect": [ 471.374149659864, 481.6326530612245, 78.0, 22.0 ],
                    "text": "mc.r~ env3 3"
                }
            },
            {
                "box": {
                    "id": "obj-40",
                    "maxclass": "newobj",
                    "numinlets": 1,
                    "numoutlets": 1,
                    "outlettype": [ "multichannelsignal" ],
                    "patching_rect": [ 123.07482993197279, 482.99319727891157, 78.0, 22.0 ],
                    "text": "mc.r~ env2 3"
                }
            },
            {
                "box": {
                    "id": "obj-41",
                    "maxclass": "newobj",
                    "numinlets": 1,
                    "numoutlets": 1,
                    "outlettype": [ "multichannelsignal" ],
                    "patching_rect": [ 36.0, 482.99319727891157, 78.0, 22.0 ],
                    "text": "mc.r~ env1 3"
                }
            },
            {
                "box": {
                    "id": "obj-27",
                    "maxclass": "newobj",
                    "numinlets": 1,
                    "numoutlets": 0,
                    "patching_rect": [ 437.3605442176871, 388.4353741496599, 70.0, 22.0 ],
                    "text": "mc.s~ env3"
                }
            },
            {
                "box": {
                    "id": "obj-24",
                    "maxclass": "newobj",
                    "numinlets": 1,
                    "numoutlets": 0,
                    "patching_rect": [ 238.0408163265306, 388.4353741496599, 70.0, 22.0 ],
                    "text": "mc.s~ env2"
                }
            },
            {
                "box": {
                    "id": "obj-22",
                    "maxclass": "newobj",
                    "numinlets": 1,
                    "numoutlets": 0,
                    "patching_rect": [ 36.0, 388.4353741496599, 70.0, 22.0 ],
                    "text": "mc.s~ env1"
                }
            },
            {
                "box": {
                    "fontsize": 14.0,
                    "id": "obj-14",
                    "linecount": 3,
                    "maxclass": "comment",
                    "numinlets": 1,
                    "numoutlets": 0,
                    "patching_rect": [ 36.0, 239.45578231292518, 714.0, 53.0 ],
                    "text": "In the help patch, three sets of three envelopes come from [number~] boxes through [mc.pack~ 3]. The first two sets feed a default two-set combinator, all three feed a three-set root combinator; compare the unpacked outputs while you change the numbers."
                }
            },
            {
                "box": {
                    "id": "obj-6",
                    "maxclass": "newobj",
                    "numinlets": 3,
                    "numoutlets": 1,
                    "outlettype": [ "multichannelsignal" ],
                    "patching_rect": [ 437.3605442176871, 353.0612244897959, 70.0, 22.0 ],
                    "text": "mc.pack~ 3"
                }
            },
            {
                "box": {
                    "fontface": 0,
                    "fontname": "Arial",
                    "fontsize": 12.0,
                    "id": "obj-7",
                    "maxclass": "number~",
                    "maximum": 1.0,
                    "minimum": 0.0,
                    "mode": 1,
                    "monitormode": 0,
                    "numinlets": 2,
                    "numoutlets": 2,
                    "outlettype": [ "signal", "float" ],
                    "patching_rect": [ 550.9659863945578, 304.76190476190476, 56.0, 22.0 ],
                    "sig": 0.969
                }
            },
            {
                "box": {
                    "fontface": 0,
                    "fontname": "Arial",
                    "fontsize": 12.0,
                    "id": "obj-8",
                    "maxclass": "number~",
                    "maximum": 1.0,
                    "minimum": 0.0,
                    "mode": 1,
                    "monitormode": 0,
                    "numinlets": 2,
                    "numoutlets": 2,
                    "outlettype": [ "signal", "float" ],
                    "patching_rect": [ 493.1428571428571, 304.76190476190476, 56.0, 22.0 ],
                    "sig": 0.0
                }
            },
            {
                "box": {
                    "fontface": 0,
                    "fontname": "Arial",
                    "fontsize": 12.0,
                    "id": "obj-9",
                    "maxclass": "number~",
                    "maximum": 1.0,
                    "minimum": 0.0,
                    "mode": 1,
                    "monitormode": 0,
                    "numinlets": 2,
                    "numoutlets": 2,
                    "outlettype": [ "signal", "float" ],
                    "patching_rect": [ 437.3605442176871, 304.76190476190476, 56.0, 22.0 ],
                    "sig": 0.5
                }
            },
            {
                "box": {
                    "fontface": 0,
                    "fontname": "Arial",
                    "fontsize": 12.0,
                    "id": "obj-50",
                    "maxclass": "number~",
                    "mode": 2,
                    "numinlets": 2,
                    "numoutlets": 2,
                    "outlettype": [ "signal", "float" ],
                    "patching_rect": [ 444.1632653061224, 633.3333333333334, 56.0, 22.0 ],
                    "sig": 0.0,
                    "sigoutmode": 0
                }
            },
            {
                "box": {
                    "fontface": 0,
                    "fontname": "Arial",
                    "fontsize": 12.0,
                    "id": "obj-51",
                    "maxclass": "number~",
                    "mode": 2,
                    "numinlets": 2,
                    "numoutlets": 2,
                    "outlettype": [ "signal", "float" ],
                    "patching_rect": [ 376.1360544217687, 633.3333333333334, 56.0, 22.0 ],
                    "sig": 0.0,
                    "sigoutmode": 0
                }
            },
            {
                "box": {
                    "fontface": 0,
                    "fontname": "Arial",
                    "fontsize": 12.0,
                    "id": "obj-52",
                    "maxclass": "number~",
                    "mode": 2,
                    "numinlets": 2,
                    "numoutlets": 2,
                    "outlettype": [ "signal", "float" ],
                    "patching_rect": [ 309.46938775510205, 633.3333333333334, 56.0, 22.0 ],
                    "sig": 0.0,
                    "sigoutmode": 0
                }
            },
            {
                "box": {
                    "fontface": 0,
                    "fontname": "Arial",
                    "fontsize": 12.0,
                    "id": "obj-49",
                    "maxclass": "number~",
                    "mode": 2,
                    "numinlets": 2,
                    "numoutlets": 2,
                    "outlettype": [ "signal", "float" ],
                    "patching_rect": [ 169.33333333333334, 634.6938775510204, 56.0, 22.0 ],
                    "sig": 0.0,
                    "sigoutmode": 0
                }
            },
            {
                "box": {
                    "fontface": 0,
                    "fontname": "Arial",
                    "fontsize": 12.0,
                    "id": "obj-48",
                    "maxclass": "number~",
                    "mode": 2,
                    "numinlets": 2,
                    "numoutlets": 2,
                    "outlettype": [ "signal", "float" ],
                    "patching_rect": [ 103.34693877551021, 634.6938775510204, 56.0, 22.0 ],
                    "sig": 0.0,
                    "sigoutmode": 0
                }
            },
            {
                "box": {
                    "fontface": 0,
                    "fontname": "Arial",
                    "fontsize": 12.0,
                    "id": "obj-47",
                    "maxclass": "number~",
                    "mode": 2,
                    "numinlets": 2,
                    "numoutlets": 2,
                    "outlettype": [ "signal", "float" ],
                    "patching_rect": [ 36.68027210884354, 634.6938775510204, 56.0, 22.0 ],
                    "sig": 0.0,
                    "sigoutmode": 0
                }
            },
            {
                "box": {
                    "id": "obj-46",
                    "maxclass": "newobj",
                    "numinlets": 1,
                    "numoutlets": 3,
                    "outlettype": [ "signal", "signal", "signal" ],
                    "patching_rect": [ 308.78911564625855, 587.7551020408164, 84.0, 22.0 ],
                    "text": "mc.unpack~ 3"
                }
            },
            {
                "box": {
                    "id": "obj-45",
                    "maxclass": "newobj",
                    "numinlets": 1,
                    "numoutlets": 3,
                    "outlettype": [ "signal", "signal", "signal" ],
                    "patching_rect": [ 36.0, 589.1156462585034, 152.0, 22.0 ],
                    "text": "mc.unpack~ 3"
                }
            },
            {
                "box": {
                    "id": "obj-39",
                    "maxclass": "newobj",
                    "numinlets": 3,
                    "numoutlets": 1,
                    "outlettype": [ "multichannelsignal" ],
                    "patching_rect": [ 238.0408163265306, 353.0612244897959, 70.0, 22.0 ],
                    "text": "mc.pack~ 3"
                }
            },
            {
                "box": {
                    "id": "obj-38",
                    "maxclass": "newobj",
                    "numinlets": 3,
                    "numoutlets": 1,
                    "outlettype": [ "multichannelsignal" ],
                    "patching_rect": [ 36.0, 353.0612244897959, 70.0, 22.0 ],
                    "text": "mc.pack~ 3"
                }
            },
            {
                "box": {
                    "fontface": 0,
                    "fontname": "Arial",
                    "fontsize": 12.0,
                    "id": "obj-35",
                    "maxclass": "number~",
                    "maximum": 1.0,
                    "minimum": 0.0,
                    "mode": 1,
                    "monitormode": 0,
                    "numinlets": 2,
                    "numoutlets": 2,
                    "outlettype": [ "signal", "float" ],
                    "patching_rect": [ 352.32653061224494, 304.76190476190476, 56.0, 22.0 ],
                    "sig": 0.0
                }
            },
            {
                "box": {
                    "fontface": 0,
                    "fontname": "Arial",
                    "fontsize": 12.0,
                    "id": "obj-36",
                    "maxclass": "number~",
                    "maximum": 1.0,
                    "minimum": 0.0,
                    "mode": 1,
                    "monitormode": 0,
                    "numinlets": 2,
                    "numoutlets": 2,
                    "outlettype": [ "signal", "float" ],
                    "patching_rect": [ 152.3265306122449, 304.76190476190476, 56.0, 22.0 ],
                    "sig": 0.0
                }
            },
            {
                "box": {
                    "fontface": 0,
                    "fontname": "Arial",
                    "fontsize": 12.0,
                    "id": "obj-32",
                    "maxclass": "number~",
                    "maximum": 1.0,
                    "minimum": 0.0,
                    "mode": 1,
                    "monitormode": 0,
                    "numinlets": 2,
                    "numoutlets": 2,
                    "outlettype": [ "signal", "float" ],
                    "patching_rect": [ 294.5034013605442, 304.76190476190476, 56.0, 22.0 ],
                    "sig": 0.891
                }
            },
            {
                "box": {
                    "fontface": 0,
                    "fontname": "Arial",
                    "fontsize": 12.0,
                    "id": "obj-33",
                    "maxclass": "number~",
                    "maximum": 1.0,
                    "minimum": 0.0,
                    "mode": 1,
                    "monitormode": 0,
                    "numinlets": 2,
                    "numoutlets": 2,
                    "outlettype": [ "signal", "float" ],
                    "patching_rect": [ 94.50340136054422, 304.76190476190476, 56.0, 22.0 ],
                    "sig": 1.0
                }
            },
            {
                "box": {
                    "fontface": 0,
                    "fontname": "Arial",
                    "fontsize": 12.0,
                    "id": "obj-31",
                    "maxclass": "number~",
                    "maximum": 1.0,
                    "minimum": 0.0,
                    "mode": 1,
                    "monitormode": 0,
                    "numinlets": 2,
                    "numoutlets": 2,
                    "outlettype": [ "signal", "float" ],
                    "patching_rect": [ 238.0408163265306, 304.76190476190476, 56.0, 22.0 ],
                    "sig": 0.95
                }
            },
            {
                "box": {
                    "fontface": 0,
                    "fontname": "Arial",
                    "fontsize": 12.0,
                    "id": "obj-30",
                    "maxclass": "number~",
                    "maximum": 1.0,
                    "minimum": 0.0,
                    "mode": 1,
                    "monitormode": 0,
                    "numinlets": 2,
                    "numoutlets": 2,
                    "outlettype": [ "signal", "float" ],
                    "patching_rect": [ 36.0, 304.76190476190476, 56.0, 22.0 ],
                    "sig": 0.764
                }
            },
            {
                "box": {
                    "id": "obj-28",
                    "maxclass": "newobj",
                    "numinlets": 3,
                    "numoutlets": 1,
                    "outlettype": [ "multichannelsignal" ],
                    "patching_rect": [ 308.78911564625855, 541.4965986394558, 155.78231292517006, 22.0 ],
                    "text": "zerr.combinator~ 3 root"
                }
            },
            {
                "box": {
                    "id": "obj-26",
                    "maxclass": "newobj",
                    "numinlets": 2,
                    "numoutlets": 1,
                    "outlettype": [ "multichannelsignal" ],
                    "patching_rect": [ 36.0, 542.8571428571429, 105.39455782312926, 22.0 ],
                    "text": "zerr.combinator~"
                }
            },
            {
                "box": {
                    "fontsize": 14.0,
                    "id": "obj-17",
                    "maxclass": "comment",
                    "numinlets": 1,
                    "numoutlets": 0,
                    "patching_rect": [ 34.63945578231293, 715.6462585034013, 705.0, 22.0 ],
                    "text": "\"max\": the largest envelope value across the sets."
                }
            },
            {
                "box": {
                    "fontsize": 14.0,
                    "id": "obj-54",
                    "maxclass": "comment",
                    "numinlets": 1,
                    "numoutlets": 0,
                    "patching_rect": [ 34.63945578231293, 736.0544217687075, 705.0, 22.0 ],
                    "text": "\"add\": the sum of the envelopes across the sets"
                }
            },
            {
                "box": {
                    "fontsize": 14.0,
                    "id": "obj-16",
                    "maxclass": "comment",
                    "numinlets": 1,
                    "numoutlets": 0,
                    "patching_rect": [ 34.63945578231293, 695.9183673469388, 710.0, 22.0 ],
                    "text": "\"root\": the geometric mean - the Nth root of the N envelopes multiplied together. Equals a square root for two sets"
                }
            },
            {
                "box": {
                    "id": "obj-29",
                    "local": 1,
                    "maxclass": "ezdac~",
                    "numinlets": 2,
                    "numoutlets": 0,
                    "patching_rect": [ 705.3877551020408, 304.76190476190476, 45.0, 45.0 ],
                    "varname": "autohelp_dac"
                }
            },
            {
                "box": {
                    "fontname": "Arial",
                    "fontsize": 12.0,
                    "id": "obj-34",
                    "linecount": 2,
                    "maxclass": "comment",
                    "numinlets": 1,
                    "numoutlets": 0,
                    "patching_rect": [ 663.8911564625851, 304.76190476190476, 40.0, 33.0 ],
                    "text": "start audio",
                    "varname": "autohelp_dac_text"
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
                    "patching_rect": [ 656.0, 23.0, 103.20000153779984, 103.20000153779984 ],
                    "pic": "zerr-logo-white-transparent.png"
                }
            },
            {
                "box": {
                    "id": "obj-19",
                    "justification": 1,
                    "maxclass": "live.line",
                    "numinlets": 1,
                    "numoutlets": 0,
                    "patching_rect": [ 38.0, 154.42176870748298, 721.2000015377998, 5.442176870748299 ]
                }
            },
            {
                "box": {
                    "fontsize": 64.0,
                    "id": "obj-10",
                    "maxclass": "comment",
                    "numinlets": 1,
                    "numoutlets": 0,
                    "patching_rect": [ 38.0, 28.571428571428573, 599.0, 78.0 ],
                    "text": "zerr.combinator~"
                }
            },
            {
                "box": {
                    "fontsize": 14.0,
                    "id": "obj-11",
                    "maxclass": "comment",
                    "numinlets": 1,
                    "numoutlets": 0,
                    "patching_rect": [ 34.63945578231293, 676.1904761904761, 745.469387755102, 22.0 ],
                    "text": "Available combination modes:"
                }
            },
            {
                "box": {
                    "fontsize": 18.0,
                    "id": "obj-23",
                    "maxclass": "comment",
                    "numinlets": 1,
                    "numoutlets": 0,
                    "patching_rect": [ 36.07823129251699, 123.12925170068027, 751.0, 27.0 ],
                    "text": "- combine several envelope sets into one: sum, geometric mean or maximum per channel"
                }
            },
            {
                "box": {
                    "fontsize": 14.0,
                    "id": "obj-25",
                    "linecount": 4,
                    "maxclass": "comment",
                    "numinlets": 1,
                    "numoutlets": 0,
                    "patching_rect": [ 36.5, 165.3061224489796, 714.0, 69.0 ],
                    "text": "The [zerr.combinator~] merges several envelope sets into one, channel by channel. The first argument sets how many envelope sets it takes (default 2), and the object gets one multichannel inlet per set. The second argument sets the combination mode, \"add\", \"root\" or \"max\" (default \"max\"). Every inlet must carry the same channel count, and the output has that many channels."
                }
            },
            {
                "box": {
                    "fontsize": 14.0,
                    "id": "obj-61",
                    "maxclass": "message",
                    "numinlets": 2,
                    "numoutlets": 1,
                    "outlettype": [ "" ],
                    "patching_rect": [ 608.7891156462584, 480.2721088435374, 78.0, 24.0 ],
                    "text": "mode max"
                }
            },
            {
                "box": {
                    "fontsize": 14.0,
                    "id": "obj-62",
                    "maxclass": "message",
                    "numinlets": 2,
                    "numoutlets": 1,
                    "outlettype": [ "" ],
                    "patching_rect": [ 694.5034013605442, 480.2721088435374, 84.0, 24.0 ],
                    "text": "mode root"
                }
            },
            {
                "box": {
                    "fontsize": 14.0,
                    "id": "obj-63",
                    "maxclass": "message",
                    "numinlets": 2,
                    "numoutlets": 1,
                    "outlettype": [ "" ],
                    "patching_rect": [ 782.2585034013605, 480.2721088435374, 78.0, 24.0 ],
                    "text": "mode add"
                }
            },
            {
                "box": {
                    "fontsize": 14.0,
                    "id": "obj-64",
                    "maxclass": "message",
                    "numinlets": 2,
                    "numoutlets": 1,
                    "outlettype": [ "" ],
                    "patching_rect": [ 570.6938775510203, 426.53061224489795, 45.0, 24.0 ],
                    "text": "bang"
                }
            }
        ],
        "lines": [
            {
                "patchline": {
                    "destination": [ "obj-45", 0 ],
                    "source": [ "obj-26", 0 ]
                }
            },
            {
                "patchline": {
                    "destination": [ "obj-46", 0 ],
                    "source": [ "obj-28", 0 ]
                }
            },
            {
                "patchline": {
                    "destination": [ "obj-38", 0 ],
                    "midpoints": [ 45.5, 359.63945578231295, 45.5, 359.63945578231295 ],
                    "source": [ "obj-30", 0 ]
                }
            },
            {
                "patchline": {
                    "destination": [ "obj-39", 0 ],
                    "source": [ "obj-31", 0 ]
                }
            },
            {
                "patchline": {
                    "destination": [ "obj-39", 1 ],
                    "midpoints": [ 304.0034013605442, 336.1700680272109, 273.0408163265306, 336.1700680272109 ],
                    "source": [ "obj-32", 0 ]
                }
            },
            {
                "patchline": {
                    "destination": [ "obj-38", 1 ],
                    "midpoints": [ 104.00340136054422, 338.55102040816325, 71.0, 338.55102040816325 ],
                    "source": [ "obj-33", 0 ]
                }
            },
            {
                "patchline": {
                    "destination": [ "obj-39", 2 ],
                    "midpoints": [ 361.82653061224494, 342.2260841836735, 298.5408163265306, 342.2260841836735 ],
                    "source": [ "obj-35", 0 ]
                }
            },
            {
                "patchline": {
                    "destination": [ "obj-38", 2 ],
                    "midpoints": [ 161.8265306122449, 346.6823979591837, 96.5, 346.6823979591837 ],
                    "source": [ "obj-36", 0 ]
                }
            },
            {
                "patchline": {
                    "destination": [ "obj-28", 2 ],
                    "midpoints": [ 480.874149659864, 522.5646258503402, 455.0714285714286, 522.5646258503402 ],
                    "source": [ "obj-37", 0 ]
                }
            },
            {
                "patchline": {
                    "destination": [ "obj-22", 0 ],
                    "source": [ "obj-38", 0 ]
                }
            },
            {
                "patchline": {
                    "destination": [ "obj-24", 0 ],
                    "source": [ "obj-39", 0 ]
                }
            },
            {
                "patchline": {
                    "destination": [ "obj-26", 1 ],
                    "source": [ "obj-40", 0 ]
                }
            },
            {
                "patchline": {
                    "destination": [ "obj-26", 0 ],
                    "source": [ "obj-41", 0 ]
                }
            },
            {
                "patchline": {
                    "destination": [ "obj-28", 1 ],
                    "midpoints": [ 398.5612244897959, 522.5646258503402, 386.6802721088436, 522.5646258503402 ],
                    "source": [ "obj-43", 0 ]
                }
            },
            {
                "patchline": {
                    "destination": [ "obj-28", 0 ],
                    "source": [ "obj-44", 0 ]
                }
            },
            {
                "patchline": {
                    "destination": [ "obj-47", 0 ],
                    "source": [ "obj-45", 0 ]
                }
            },
            {
                "patchline": {
                    "destination": [ "obj-48", 0 ],
                    "source": [ "obj-45", 1 ]
                }
            },
            {
                "patchline": {
                    "destination": [ "obj-49", 0 ],
                    "source": [ "obj-45", 2 ]
                }
            },
            {
                "patchline": {
                    "destination": [ "obj-50", 0 ],
                    "midpoints": [ 383.28911564625855, 620.4281462585035, 453.6632653061224, 620.4281462585035 ],
                    "source": [ "obj-46", 2 ]
                }
            },
            {
                "patchline": {
                    "destination": [ "obj-51", 0 ],
                    "midpoints": [ 350.78911564625855, 624.7329931972789, 385.6360544217687, 624.7329931972789 ],
                    "source": [ "obj-46", 1 ]
                }
            },
            {
                "patchline": {
                    "destination": [ "obj-52", 0 ],
                    "source": [ "obj-46", 0 ]
                }
            },
            {
                "patchline": {
                    "destination": [ "obj-27", 0 ],
                    "source": [ "obj-6", 0 ]
                }
            },
            {
                "patchline": {
                    "destination": [ "obj-28", 0 ],
                    "midpoints": [ 618.2891156462584, 532.3662840136061, 318.28911564625855, 532.3662840136061 ],
                    "source": [ "obj-61", 0 ]
                }
            },
            {
                "patchline": {
                    "destination": [ "obj-28", 0 ],
                    "midpoints": [ 704.0034013605442, 532.254676870749, 318.28911564625855, 532.254676870749 ],
                    "source": [ "obj-62", 0 ]
                }
            },
            {
                "patchline": {
                    "destination": [ "obj-28", 0 ],
                    "midpoints": [ 791.7585034013605, 532.6399872448985, 318.28911564625855, 532.6399872448985 ],
                    "source": [ "obj-63", 0 ]
                }
            },
            {
                "patchline": {
                    "destination": [ "obj-28", 0 ],
                    "midpoints": [ 580.1938775510203, 521.6885629251702, 318.28911564625855, 521.6885629251702 ],
                    "source": [ "obj-64", 0 ]
                }
            },
            {
                "patchline": {
                    "destination": [ "obj-6", 2 ],
                    "midpoints": [ 560.4659863945578, 341.589710884354, 497.8605442176871, 341.589710884354 ],
                    "source": [ "obj-7", 0 ]
                }
            },
            {
                "patchline": {
                    "destination": [ "obj-6", 1 ],
                    "midpoints": [ 502.6428571428571, 334.96502976190493, 472.3605442176871, 334.96502976190493 ],
                    "source": [ "obj-8", 0 ]
                }
            },
            {
                "patchline": {
                    "destination": [ "obj-6", 0 ],
                    "source": [ "obj-9", 0 ]
                }
            }
        ],
        "autosave": 0
    }
}