
try:
    import artifactory_utils
except:
    pass
else:
    dependencies = [
        artifactory_utils.ArtifactSelector(
            project="Toolchain-Release",
            revision="master",
            version="3.9.*",
            debug=False,
            stable_required=True),
        artifactory_utils.ArtifactSelector(
            project="json-cpp",
            revision="master",
            version="3.8.*",
            debug=False,
            stable_required=True)
    ]
