{
	"patcher": {
		"fileversion": 1,
		"appversion": {
			"major": 9,
			"minor": 0,
			"revision": 0,
			"architecture": "x64",
			"modernui": 1
		},
		"classnamespace": "box",
		"rect": [
			80.0,
			100.0,
			940.0,
			640.0
		],
		"gridsize": [
			15.0,
			15.0
		],
		"boxes": [
			{
				"box": {
					"id": "obj-1",
					"maxclass": "comment",
					"numinlets": 1,
					"numoutlets": 0,
					"patching_rect": [
						22.0,
						16.0,
						700.0,
						40.0
					],
					"text": "Zerr*: Autogenous Spatialization Toolbox",
					"fontsize": 30.0,
					"textcolor": [
						0.647058823529412,
						0.647058823529412,
						0.647058823529412,
						1.0
					]
				}
			},
			{
				"box": {
					"id": "obj-2",
					"maxclass": "comment",
					"numinlets": 1,
					"numoutlets": 0,
					"patching_rect": [
						22.0,
						60.0,
						900.0,
						40.0
					],
					"text": "Channel-based spatialization for any loudspeaker layout. Audio features extracted from the signal drive its distribution over the speakers: no manual positioning. Four objects form the chain; each has a help patch.",
					"fontsize": 13.0
				}
			},
			{
				"box": {
					"id": "obj-3",
					"maxclass": "live.line",
					"numinlets": 1,
					"numoutlets": 0,
					"patching_rect": [
						22.0,
						106.0,
						900.0,
						7.0
					],
					"justification": 1
				}
			},
			{
				"box": {
					"id": "obj-4",
					"maxclass": "comment",
					"numinlets": 1,
					"numoutlets": 0,
					"patching_rect": [
						22.0,
						120.0,
						900.0,
						22.0
					],
					"text": "source  ->  zerr.features~  ->  zerr.envelopes~  ->  zerr.combinator~  ->  zerr.disperser~  ->  mc.dac~",
					"fontsize": 14.0,
					"fontface": 1
				}
			},
			{
				"box": {
					"id": "obj-5",
					"maxclass": "newobj",
					"numinlets": 1,
					"numoutlets": 1,
					"patching_rect": [
						780.0,
						500.0,
						56.0,
						22.0
					],
					"outlettype": [
						""
					],
					"text": "pcontrol"
				}
			},
			{
				"box": {
					"id": "obj-6",
					"maxclass": "comment",
					"numinlets": 1,
					"numoutlets": 0,
					"patching_rect": [
						22.0,
						160.0,
						180.0,
						24.0
					],
					"text": "zerr.features~",
					"fontsize": 16.0,
					"fontface": 1
				}
			},
			{
				"box": {
					"id": "obj-7",
					"maxclass": "comment",
					"numinlets": 1,
					"numoutlets": 0,
					"patching_rect": [
						200.0,
						160.0,
						560.0,
						64.0
					],
					"text": "Extracts audio features from a mono signal, at audio rate. Arguments pick the features (rms zcr flx ctd rlf cf flt zc); one signal outlet per argument, in argument order. These are the control signals that drive the spatialization.",
					"fontsize": 12.0
				}
			},
			{
				"box": {
					"id": "obj-8",
					"maxclass": "message",
					"numinlets": 2,
					"numoutlets": 1,
					"patching_rect": [
						780.0,
						160.0,
						140.0,
						22.0
					],
					"outlettype": [
						""
					],
					"text": "help zerr.features~"
				}
			},
			{
				"box": {
					"id": "obj-9",
					"maxclass": "comment",
					"numinlets": 1,
					"numoutlets": 0,
					"patching_rect": [
						22.0,
						242.0,
						180.0,
						24.0
					],
					"text": "zerr.envelopes~",
					"fontsize": 16.0,
					"fontface": 1
				}
			},
			{
				"box": {
					"id": "obj-10",
					"maxclass": "comment",
					"numinlets": 1,
					"numoutlets": 0,
					"patching_rect": [
						200.0,
						242.0,
						560.0,
						64.0
					],
					"text": "Turns a control signal plus a speaker-layout YAML into one envelope per loudspeaker. 'trigger' mode jumps to another speaker on each pulse; 'trajectory' mode pans along a chain of speakers. Spread and gain are audio-rate inlets; the active, traj, topo, curr and interval messages reshape the behaviour while it runs.",
					"fontsize": 12.0
				}
			},
			{
				"box": {
					"id": "obj-11",
					"maxclass": "message",
					"numinlets": 2,
					"numoutlets": 1,
					"patching_rect": [
						780.0,
						242.0,
						140.0,
						22.0
					],
					"outlettype": [
						""
					],
					"text": "help zerr.envelopes~"
				}
			},
			{
				"box": {
					"id": "obj-12",
					"maxclass": "comment",
					"numinlets": 1,
					"numoutlets": 0,
					"patching_rect": [
						22.0,
						324.0,
						180.0,
						24.0
					],
					"text": "zerr.combinator~",
					"fontsize": 16.0,
					"fontface": 1
				}
			},
			{
				"box": {
					"id": "obj-13",
					"maxclass": "comment",
					"numinlets": 1,
					"numoutlets": 0,
					"patching_rect": [
						200.0,
						324.0,
						560.0,
						64.0
					],
					"text": "Merges several envelope sets (one multichannel inlet per set, equal channel counts) into one set by add, root (geometric mean) or max. The mode is an argument, an attribute, and a message, so it can change while audio runs.",
					"fontsize": 12.0
				}
			},
			{
				"box": {
					"id": "obj-14",
					"maxclass": "message",
					"numinlets": 2,
					"numoutlets": 1,
					"patching_rect": [
						780.0,
						324.0,
						140.0,
						22.0
					],
					"outlettype": [
						""
					],
					"text": "help zerr.combinator~"
				}
			},
			{
				"box": {
					"id": "obj-15",
					"maxclass": "comment",
					"numinlets": 1,
					"numoutlets": 0,
					"patching_rect": [
						22.0,
						406.0,
						180.0,
						24.0
					],
					"text": "zerr.disperser~",
					"fontsize": 16.0,
					"fontface": 1
				}
			},
			{
				"box": {
					"id": "obj-16",
					"maxclass": "comment",
					"numinlets": 1,
					"numoutlets": 0,
					"patching_rect": [
						200.0,
						406.0,
						560.0,
						64.0
					],
					"text": "Multiplies a mono source by a multichannel envelope set: one output channel per loudspeaker, ready for mc.dac~ or further routing.",
					"fontsize": 12.0
				}
			},
			{
				"box": {
					"id": "obj-17",
					"maxclass": "message",
					"numinlets": 2,
					"numoutlets": 1,
					"patching_rect": [
						780.0,
						406.0,
						140.0,
						22.0
					],
					"outlettype": [
						""
					],
					"text": "help zerr.disperser~"
				}
			},
			{
				"box": {
					"id": "obj-18",
					"maxclass": "live.line",
					"numinlets": 1,
					"numoutlets": 0,
					"patching_rect": [
						22.0,
						490.0,
						900.0,
						7.0
					],
					"justification": 1
				}
			},
			{
				"box": {
					"id": "obj-19",
					"maxclass": "comment",
					"numinlets": 1,
					"numoutlets": 0,
					"patching_rect": [
						22.0,
						504.0,
						740.0,
						50.0
					],
					"text": "Speaker layouts ship in the package's misc/ folder (quad_4, ring_8, line_16, ambisonic_21) and circulation_8 next to the help patches; name one as the second argument of zerr.envelopes~. Write your own YAML with a cartesian or spherical position per speaker.",
					"fontsize": 12.0
				}
			},
			{
				"box": {
					"id": "obj-20",
					"maxclass": "comment",
					"numinlets": 1,
					"numoutlets": 0,
					"patching_rect": [
						22.0,
						558.0,
						740.0,
						22.0
					],
					"text": "Z. Yang, H. von Coler. Autogenous Spatialization for Arbitrary Loudspeaker Setups (I3DA 2023).  github.com/ringbuffer-org/Zerr",
					"fontsize": 11.0,
					"textcolor": [
						0.647058823529412,
						0.647058823529412,
						0.647058823529412,
						1.0
					]
				}
			},
			{
				"box": {
					"id": "obj-21",
					"maxclass": "comment",
					"numinlets": 1,
					"numoutlets": 0,
					"patching_rect": [
						780.0,
						526.0,
						140.0,
						36.0
					],
					"text": "click a 'help' box to open the object's help patch",
					"fontsize": 10.0,
					"textcolor": [
						0.647058823529412,
						0.647058823529412,
						0.647058823529412,
						1.0
					]
				}
			}
		],
		"lines": [
			{
				"patchline": {
					"destination": [
						"obj-5",
						0
					],
					"source": [
						"obj-8",
						0
					]
				}
			},
			{
				"patchline": {
					"destination": [
						"obj-5",
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
						"obj-5",
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
						"obj-5",
						0
					],
					"source": [
						"obj-17",
						0
					]
				}
			}
		],
		"dependency_cache": [],
		"autosave": 0
	}
}
