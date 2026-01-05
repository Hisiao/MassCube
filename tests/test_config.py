from app.config import AppConfig, ConfigStore


def test_config_roundtrip(tmp_path):
    cfg = AppConfig()
    store = ConfigStore(config_path=tmp_path / "cfg.yaml", version_log=tmp_path / "ver.json")
    store.config = cfg
    store.update(cfg.model_dump(mode="json"))
    loaded = ConfigStore(config_path=tmp_path / "cfg.yaml", version_log=tmp_path / "ver.json")
    loaded.load()
    assert loaded.config.orbits.refresh_rate_sec == cfg.orbits.refresh_rate_sec
    assert loaded.version == 1

